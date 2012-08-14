/*
 *  sinker.cpp
 *  pybg
 *
 *  Created by BART MOSLEY on 8/12/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */


#include <bg/bondgeek.hpp>

#include <iostream>

#include <boost/timer.hpp>

using namespace std;
using namespace QuantLib;
using namespace bondgeek;

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {
    Integer sessionId() { return 0; }
}
#endif

std::pair<Integer,Integer> daysMinMax(const Period& p) {
    switch (p.units()) {
        case Days:
            return std::make_pair(p.length(), p.length());
        case Weeks:
            return std::make_pair(7*p.length(), 7*p.length());
        case Months:
            return std::make_pair(28*p.length(), 31*p.length());
        case Years:
            return std::make_pair(365*p.length(), 366*p.length());
        default:
            QL_FAIL("unknown time unit (" << Integer(p.units()) << ")");
    }
}

Integer numSubPeriod(const Period& subPeriod,
                    const Period& superPeriod
                    ) 
{
    Integer numSubPeriods;
    
    std::pair<Integer, Integer> superDays(daysMinMax(superPeriod));
    std::pair<Integer, Integer> subDays(daysMinMax(subPeriod));
    
    //obtain the approximate time ratio
    Real minPeriodRatio =
    ((Real)superDays.first)/((Real)subDays.second);
    Real maxPeriodRatio =
    ((Real)superDays.second)/((Real)subDays.first);
    Integer lowRatio = static_cast<Integer>(std::floor(minPeriodRatio));
    Integer highRatio = static_cast<Integer>(std::ceil(maxPeriodRatio));
    
    try {
        for(Integer i=lowRatio; i <= highRatio; ++i) {
            Period testPeriod = subPeriod * i;
            if(testPeriod == superPeriod) {
                numSubPeriods = i;
                return numSubPeriods;
            }
        }
    } catch(Error e) {
        return -1;
    }
    
    return 0;
}

std::vector<Real> sinkingFundNotionals(const std::vector<Real>& sf_par,
                                        Frequency sf_freq,
                                        Frequency frequency,
                                        int sched_size
                                        )
{
    std::vector<Real> sf_notionals;
    
    //Total amount sunk
    double sf_total=0.0;
    for (int i=0; i < sf_par.size(); i++)
        sf_total += sf_par[i];
    
    int nsub = numSubPeriod(Period(frequency), Period(sf_freq));
    QL_ENSURE(nsub > 0, "sink frequency not compatible with bond schedule");
    
    int sf_start_pos = sched_size - (nsub * sf_par.size()); 
    
    // prior to sinking fund start all notionals are par.
    for (int i = 0; i < sf_start_pos; i++)
    {
        sf_notionals.push_back(100.);
    }
    
    double bal = 100. ;
    int n_itr = 0;
    for (int i=sf_start_pos; i < sched_size; i=i+nsub) 
    {
        for (int j = 0; j < nsub; j++) 
        {
            sf_notionals.push_back(bal);
        }
        
        bal -= 100.0 * sf_par[n_itr] / sf_total;
        n_itr++;        
    }
    
    return sf_notionals;
}

class NonCallBond : public Bond {
protected:
    Frequency frequency_;
    DayCounter dayCounter_;
    
public:
    /* Generic Fixed Rate Bond, no amortization 
     */
    NonCallBond(
                Natural                     settlementDays,
                Real                        faceAmount,
                const Schedule&             schedule,
                const std::vector<Rate>&    coupons,
                const DayCounter&           accrualDayCounter,
                BusinessDayConvention       paymentConvention = Following,
                Real                        redemption = 100.0,
                const Date&                 issueDate = Date(),
                const Calendar&             paymentCalendar = Calendar()
                ) : 
    Bond(settlementDays,
         paymentCalendar==Calendar() ? schedule.calendar() : paymentCalendar,
         issueDate),
    frequency_(schedule.tenor().frequency()),
    dayCounter_(accrualDayCounter) 
    {
        maturityDate_ = schedule.endDate();
        
        cashflows_ = FixedRateLeg(schedule)
        .withNotionals(faceAmount)
        .withCouponRates(coupons, accrualDayCounter)
        //.withPaymentCalendar(calendar_)
        .withPaymentAdjustment(paymentConvention);
        
        addRedemptionsToCashflows(std::vector<Real>(1, redemption));
    }
    
    /* Generic Fixed Rate Amortizing Bond 
       
    notionals:  A declining balance (actually non-increasing) balance 
     of outstanding notional at each schedule date.
    
    redemptions: A vector of redemption prices for each amortization, assumed to
     be par (100.) if not provided. 
    */
    NonCallBond(
                Natural                     settlementDays,
                const std::vector<Real>&    notionals,
                const Schedule&             schedule,
                const std::vector<Rate>&    coupons,
                const DayCounter&           accrualDayCounter,
                BusinessDayConvention       paymentConvention = Following,
                const std::vector<Real>&    redemptionValues = std::vector<Real>(),
                const Date&                 issueDate = Date(),
                const Calendar&             paymentCalendar = Calendar()
                ) : 
    Bond(settlementDays,
         paymentCalendar==Calendar() ? schedule.calendar() : paymentCalendar,
         issueDate),
    frequency_(schedule.tenor().frequency()),
    dayCounter_(accrualDayCounter) 
    {
        maturityDate_ = schedule.endDate();
        
        cashflows_ = FixedRateLeg(schedule)
        .withNotionals(notionals)
        .withCouponRates(coupons, accrualDayCounter)
        //check quantlib version
        //.withPaymentCalendar(calendar_)
        .withPaymentAdjustment(paymentConvention);

        addRedemptionsToCashflows(redemptionValues);

    }

    
    void showCashFlows(void)
    {
        Leg fixedLeg = this->cashflows();
        
        cout << endl << "Fixed Leg: " << endl;
        Leg::iterator fxIt;
        Date cfDate;
        double cfAmt;
        for (fxIt=fixedLeg.begin(); fxIt < fixedLeg.end(); fxIt++) 
        {
            cfDate = (*fxIt)->date();
            cfAmt = (*fxIt)->amount();
            cout << cfDate << " | " << cfAmt << endl;
        }
        
    }

    void showRedemptions(void)
    {
        Leg fixedLeg = this->redemptions();
        
        cout << endl << "Redemptions: " << endl;
        Leg::iterator fxIt;
        Date cfDate;
        double cfAmt;
        for (fxIt=fixedLeg.begin(); fxIt < fixedLeg.end(); fxIt++) 
        {
            cfDate = (*fxIt)->date();
            cfAmt = (*fxIt)->amount();
            cout << cfDate << " | " << cfAmt << endl;
        }
        
    }
    
    void showNotionals(void)
    {
        std::vector<Real> bnd_notionals = this->notionals();
        
        cout << endl << "Notionals: " << endl;
        std::vector<Real>::iterator it;
        int notional_count = 0;
        for (it=bnd_notionals.begin(); it < bnd_notionals.end(); it++) {
            notional_count++;
            cout << notional_count << ": " << *it << endl;
        }
        
    }
};
    

int main (int argc, char * const argv[]) 
{
    
    Date today = Date(16,October,2007);
    Settings::instance().evaluationDate() = today;

    cout << endl << "Today: " << today << endl;
    cout << "Price with a bg curve " << endl;
    string depotenors[] = {"1W", "1M", "3M", "6M", "9M", "1y"};
    double depospots[] = {.055, .055, .055, .055, .055, .055};
    string swaptenors[] = {"2y", "3y", "5y", "10y", "15y", "20y", "30y"};
    double swapspots[] = {.055, .055, .055, .055, .055, .055, .055};
    
    cout << "Test with new curve " << endl;
    RateHelperCurve usdLiborCurve = RateHelperCurve(USDLiborCurve("3M"));
    usdLiborCurve.update(depotenors, 
                         depospots, 
                         6,
                         swaptenors,                                                                      
                         swapspots,
                         7,
                         today);
    cout << "df: " << usdLiborCurve.discount(10.) << endl;
    
    
    // set up the callable bond
    Real coupon = .06;
    Date maturity(15,September,2012);
    
    Date dated(16,September,2004);
    
    Natural settlementDays = 3;  // Bloomberg OAS1 settle is Oct 19, 2007
    Frequency frequency = Semiannual;
    
    Calendar bondCalendar = UnitedStates(UnitedStates::GovernmentBond);
    DayCounter bondDayCounter = ActualActual(ActualActual::Bond);
    
    Real faceAmount = 100.0; // Not
    Real redemption = 100.0;
    
    BusinessDayConvention accrualConvention = Unadjusted;
    BusinessDayConvention paymentConvention = Unadjusted;
    
    Schedule sch(dated, maturity, Period(frequency), bondCalendar,
                 accrualConvention, accrualConvention,
                 DateGeneration::Backward, false);
    
    cout << endl << endl
    << "Generic Bond"
    << endl;
    NonCallBond bnd(
                settlementDays,
                faceAmount,
                sch,
                std::vector<Rate>(1, coupon),
                bondDayCounter,
                paymentConvention,
                redemption,
                dated
                );
    bnd.showCashFlows();
    bnd.showNotionals();
    
    
    /* Sinkfing fund amortization:
     
     notionals start at par on the dated date and end at zero on maturity.
     redemptions are the prices redemption prices (pct of par, e.g. 101.)
     
     */
    
    double sf_sch[] = {
        40000, 40000, 40000, 40000    };
    int sf_num = 4;
    Frequency sf_freq = Annual;
    
    std::vector<Real> sf_notionals;
    std::vector<Real> sf_redemptions;
    
    std::vector<double> sf_bal;
    sf_bal.assign(sf_sch, sf_sch+sf_num);
    
    sf_notionals = sinkingFundNotionals(sf_bal, sf_freq, frequency, sch.size());
    
    //redeem the last sinker, for example, at 101.
    Real rval = 101.;
    
    for (int i=0; i < sf_notionals.size(); i++) 
    {
        sf_redemptions.push_back(i == sch.size()-1 ? rval : 100.0 );
    }
     
    cout << endl << endl
    << "Amortizing Generic Bond "
    << endl;
    NonCallBond bnd2(
                 settlementDays,
                 sf_notionals,
                 sch,
                 std::vector<Rate>(1, coupon),
                 bondDayCounter,
                 paymentConvention,
                 sf_redemptions,
                 dated
                 ); 
    
    // show the results
    bnd2.showCashFlows();
    bnd2.showRedemptions();
    bnd2.showNotionals();
    
    return 0;
}

