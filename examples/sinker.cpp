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
        .withPaymentCalendar(calendar_)
        .withPaymentAdjustment(paymentConvention);
        
        addRedemptionsToCashflows();
    }
    
    /* Fixed Rate amortizing bond 
       
    notionals:  A declining balance (actually non-increasing) balance 
     of outstanding notional at each schedule date.
    
    redemptions: A vector of redemption prices for each amortization, assumed to
     be par (100.) if not provided. 
    */
    NonCallBond(Natural                     settlementDays,
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
        .withPaymentCalendar(calendar_)
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
    
    std::vector<Real> sinkingfund;
    std::vector<Real> redvals;
    
    double sf_sch = {
        40000, 40000, 40000, 40000, 40000, 40000,
        40000, 40000, 40000, 40000, 40000, 40000,
        40000
    }
    
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
        redvals.push_back(rval);
    }
    cout << "size: " << sinkingfund.size() 
    << " amount: " << notional_amt
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
                 redvals,
                 dated
                 );
    
    bnd2.showCashFlows();
    
    bnd2.showRedemptions();
    
    cout << "Show notionals " << endl;
    std::vector<Real> bnd_notionals = bnd2.notionals();
    std::vector<Real>::iterator it;
    int notional_count = 0;
    for (it=bnd_notionals.begin(); it < bnd_notionals.end(); it++) {
        notional_count++;
        cout << notional_count << ": " << *it << endl;
    }
    
    return 0;
}