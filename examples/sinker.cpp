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

void showCashFlows(Bond &bnd)
{
    Leg fixedLeg = bnd.cashflows();
    
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

void showRedemptions(Bond &bnd)
{
    Leg fixedLeg = bnd.redemptions();
    
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

void showNotionals(Bond &bnd)
{
    std::vector<Real> bnd_notionals = bnd.notionals();
    
    cout << endl << "Notionals: " << endl;
    std::vector<Real>::iterator it;
    int notional_count = 0;
    for (it=bnd_notionals.begin(); it < bnd_notionals.end(); it++) {
        notional_count++;
        cout << notional_count << ": " << *it << endl;
    }
    
}

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
                                       Frequency                sf_freq,
                                       const Schedule&          schedule
                                       )
{
    Frequency   frequency = schedule.tenor().frequency();
    int         sched_size = schedule.size();
    
    std::vector<Real> sf_notionals(sched_size, 0.0);
    
    //Total amount sunk
    double sf_total=0.0;
    for (int i=0; i < sf_par.size(); i++)
        sf_total += sf_par[i];
    
    int nsub = numSubPeriod(Period(frequency), Period(sf_freq));
    QL_ENSURE(nsub > 0, "sink frequency not compatible with bond schedule");
    QL_ENSURE(sf_par.size() <= sched_size, "sink schedule longer than bond schedule");
    
    // prior to sinking fund start all notionals are par.
    int sf_start_pos = sched_size - (1 + nsub * sf_par.size()); 
    for (int i = 0; i < sf_start_pos; i++)
    {
        sf_notionals[i] = 100.;
    }
    
    double bal = 100. ;
    int n_itr = 0;
    for (int i=sf_start_pos; i < sched_size; i=i+nsub) 
    {
        for (int j = 0; j < nsub; j++) 
        {
            if (i+j < sched_size)
                sf_notionals[i+j] = bal;
        }
        
        bal -= 100.0 * sf_par[n_itr] / sf_total;
        n_itr++;        
    }
    
    return sf_notionals;
}

class SinkingFundBond : public Bond {
protected:
    Frequency frequency_;
    DayCounter dayCounter_;
    
public:
    /* Generic Fixed Rate Amortizing Bond 
       
    notionals:  A declining balance (actually non-increasing) balance 
     of outstanding notional at each schedule date.
    
    redemptions: A vector of redemption prices for each amortization, assumed to
     be par (100.) if not provided. 
    */
    SinkingFundBond(
                Natural                     settlementDays,
                const std::vector<Real>&    notionalAmortization,
                const Schedule&             schedule,
                const std::vector<Rate>&    coupons,
                const DayCounter&           accrualDayCounter,
                BusinessDayConvention       paymentConvention = Following,
                Real                        redemptionAmount = 100.0,
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
        .withNotionals(notionalAmortization)
        .withCouponRates(coupons, accrualDayCounter)
        //check quantlib version
        //.withPaymentCalendar(calendar_)
        .withPaymentAdjustment(paymentConvention);

        addRedemptionsToCashflows(std::vector<Real>(1, redemptionAmount));        
        
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
    
    boost::shared_ptr<PricingEngine> discEngine = \
    createPriceEngine<DiscountingBondEngine>(
                                             usdLiborCurve.discountingTermStructure()
                                             );
    
    // set up the callable bond
    Real coupon = .06;
    Date maturity(15, September,2012);
    Date maturity1(15, September, 2011);
    Date maturity0(15, September, 2010);
    
    Date dated(16,September,2004);
    
    Natural settlementDays = 3;  // Bloomberg OAS1 settle is Oct 19, 2007
    Frequency frequency = Semiannual;
    
    Calendar bondCalendar = UnitedStates(UnitedStates::GovernmentBond);
    DayCounter bondDayCounter = ActualActual(ActualActual::Bond);
    
    Real faceAmount = 100.0; // Notional amount
    Real redemption = 100.0; // Amount paid on redemption
    
    BusinessDayConvention accrualConvention = Unadjusted;
    BusinessDayConvention paymentConvention = Unadjusted;
    
    Schedule sch(dated, maturity, Period(frequency), bondCalendar,
                 accrualConvention, accrualConvention,
                 DateGeneration::Backward, false);
    
    cout << endl << endl
    << "Generic Bond"
    << endl;
    SinkingFundBond bnd(
                        settlementDays,
                        std::vector<Real>(1,faceAmount),
                        sch,
                        std::vector<Rate>(1, coupon),
                        bondDayCounter,
                        paymentConvention,
                        redemption,
                        dated
                        );
    showCashFlows(bnd);
    
    cout << "\n\none year shorter" << endl;
    SinkingFundBond bnd0(
                         settlementDays,
                         std::vector<Real>(1,faceAmount),
                         Schedule(dated, maturity0, Period(frequency), bondCalendar,
                                  accrualConvention, accrualConvention,
                                  DateGeneration::Backward, false),
                         std::vector<Rate>(1, coupon),
                         bondDayCounter,
                         paymentConvention,
                         redemption,
                         dated
                         );
    showCashFlows(bnd0);
    
    cout << "\n\ntwo years shorter" << endl;
    SinkingFundBond bnd1(
                         settlementDays,
                         std::vector<Real>(1,faceAmount),
                         Schedule(dated, maturity1, Period(frequency), bondCalendar,
                                  accrualConvention, accrualConvention,
                                  DateGeneration::Backward, false),
                         std::vector<Rate>(1, coupon),
                         bondDayCounter,
                         paymentConvention,
                         redemption,
                         dated
                         );
    showCashFlows(bnd1);
    
    /* Sinkfing fund amortization:
     
     notionals start at par on the dated date and end at zero on maturity.
     redemptions are the prices redemption prices (pct of par, e.g. 101.)
     
     */
    
    double sf_sch[] = {
        40000, 40000, 40000
    };
    int sf_num = 3;
    Frequency sf_freq = Annual;
    
    std::vector<Real> sf_notionals;
    std::vector<Real> sf_redemptions;
    
    std::vector<double> sf_bal;
    sf_bal.assign(sf_sch, sf_sch+sf_num);
    
    sf_notionals = sinkingFundNotionals(sf_bal, sf_freq, sch);
    
    //redeem the last sinker, for example, at 101.
    Real rval = 100.;
         
    cout << endl << endl
    << "Amortizing Generic Bond "
    << endl;
    SinkingFundBond bnd2(
                 settlementDays,
                 sf_notionals,
                 sch,
                 std::vector<Rate>(1, coupon),
                 bondDayCounter,
                 paymentConvention,
                 rval,
                 dated
                 ); 
    
    // show the results
    showCashFlows(bnd2);
    showRedemptions(bnd2);
    
    bnd0.setPricingEngine(discEngine);
    bnd1.setPricingEngine(discEngine);
    bnd.setPricingEngine(discEngine);
    bnd2.setPricingEngine(discEngine);
    
    double prc0 = bnd0.cleanPrice();
    double prc1 = bnd1.cleanPrice();
    double prc = bnd.cleanPrice();
    double prc2 = bnd2.cleanPrice();
    
    cout << endl << "priced0: " << prc0 << endl; 
    cout << endl << "priced1: " << prc1 << endl; 
    cout << endl << "priced: " << prc << endl; 
    cout << endl << "priced2: " << prc2 << endl;
    
    cout << "avg price: " << (prc0+prc1+prc)/3. << endl;


    
    return 0;
}

