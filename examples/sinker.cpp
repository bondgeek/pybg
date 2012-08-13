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

class NonCallBond : public Bond {
protected:
    Frequency frequency_;
    DayCounter dayCounter_;
    
public:
    /* Generic Fixed Rate Bond, no amortization 
     */
    NonCallBond(Natural                     settlementDays,
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
        
        addRedemptionsToCashflows();
    }
    
    /* Fixed Rate amortizing bond 
       
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
    
    std::vector<Real> sinkingfund;
    std::vector<Real> sf_notionals;
    
    double sf_sch[] = {
        40000, 40000, 40000, 40000    };
    int sf_num = 4;
    Frequency sf_freq = Annual;
    
    int nsub = numSubPeriod(Period(frequency), Period(sf_freq));
    cout << endl << endl << Period(frequency) << " | " << Period(sf_freq) 
    << endl 
    << "numSubPeriods: " << nsub << endl << endl;

    std::vector<double> redvals;
    redvals.assign(sf_sch, sf_sch+sf_num);
    
    double sf_total=0.0;
    for (std::vector<double>::iterator it=redvals.begin();
         it < redvals.end(); it++)
        sf_total += *it;
    
    cout << endl << endl << "sf total: " << sf_total ;
    
    double sf_not=100.; // you start with par
    double amt_sunk;
    for (std::vector<double>::iterator it=redvals.begin();
         it < redvals.end(); ++it) 
    {
        amt_sunk = 100. * (*it/sf_total);
        
        *it = sf_not ;
        
        sf_not -= amt_sunk;
    }
    redvals.push_back(0.0);  // and you end up with no balance.
    
    cout << "\n\nNotionals: " << endl;
    for (std::vector<double>::iterator it=redvals.begin();
         it < redvals.end(); it++)
        cout <<  *it << endl;
    
    /*
    int n_itr = redvals.size();
    int n_subitr = 0;
    double notional_bal = 0.;
    for (int i=sch.size()-1; i>=0; i--) 
    {
        cout << "adding " << i << " of " 
        << sch.size() << ": ";
        
        if (--n_subitr <= 0 && notional_bal < 100.0) { 
            n_subitr = nsub;
            notional_bal = redvals[--n_itr];
        }
        sf_notionals.push_back(notional_bal);
        
        cout << notional_bal << endl;
    }
    */ 
    
    double notional_amt = 100.;
    double sfamt = 100.00/sch.size();
    Real   rval;
    
    for (int i=0; i < sch.size(); i++) {
        cout << "adding " << i << " of " 
        << sch.size() << ": ";
    
        sinkingfund.push_back(notional_amt);
        cout << notional_amt << endl;
        notional_amt -= sfamt;
        
        //redeem the last sinker at 101.
        rval = i == sch.size()-1 ? 101.0 : 100.0 ;
        sf_notionals.push_back(rval);
    }
    
    for (std::vector<Real>::iterator it=sf_notionals.begin();
         it < sf_notionals.end(); it++)
        cout <<  *it << endl;
    
    cout << "size: " << sf_notionals.size()
    << endl;

    
    cout << endl << endl
    << "Amortizing Generic Bond "
    << endl;
    NonCallBond bnd2(
                 settlementDays,
                 sinkingfund,
                 sch,
                 std::vector<Rate>(1, coupon),
                 bondDayCounter,
                 paymentConvention,
                 sf_notionals,
                 dated
                 );
    cout << "built " << endl;
    bnd2.showCashFlows();
    
    bnd2.showRedemptions();
    bnd2.showNotionals();
    
    return 0;
}