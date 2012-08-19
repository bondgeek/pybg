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
    
    cout << endl << endl
    << "Generic Bond"
    << endl;
    SinkingFundBond bnd(coupon,
                        maturity,
                        std::vector<Real>(1, faceAmount),
                        Annual,
                        dated,
                        bondCalendar,
                        settlementDays,
                        bondDayCounter,
                        frequency,
                        redemption,
                        faceAmount,
                        accrualConvention,
                        paymentConvention
                        );
    
    showCashFlows(bnd);

    cout << "\n\none year shorter" << endl;
    SinkingFundBond bnd0(coupon,
                         maturity0,
                         std::vector<Real>(1, faceAmount),
                         Annual,
                         dated,
                         bondCalendar,
                         settlementDays,
                         bondDayCounter,
                         frequency,
                         redemption,
                         faceAmount,
                         accrualConvention,
                         paymentConvention
                         );
    
    showCashFlows(bnd0);
    
    cout << "\n\ntwo years shorter" << endl;
    SinkingFundBond bnd1(coupon,
                         maturity1,
                         std::vector<Real>(1, faceAmount),
                         Annual,
                         dated,
                         bondCalendar,
                         settlementDays,
                         bondDayCounter,
                         frequency,
                         redemption,
                         faceAmount,
                         accrualConvention,
                         paymentConvention
                         );
    showCashFlows(bnd1);

    /* Sinkfing fund amortization:
     
     notionals start at par on the dated date and end at zero on maturity.
     redemptions are the prices redemption prices (pct of par, e.g. 101.)

     */
    
    double sf_sch[] = {
        40000, 40000, 40000, 40000, 40000, 40000,
        40000, 40000, 40000, 40000, 40000, 40000,
        40000
    };
    int sf_num = 3;
    Frequency sf_freq = Annual;
    
    std::vector<double> sf_bal;
    sf_bal.assign(sf_sch, sf_sch+sf_num);
    
    Real rval = 100.;
         
    cout << endl << endl
    << "Amortizing Generic Bond "
    << endl;
    SinkingFundBond bnd2(coupon,
                         maturity,
                         sf_bal,
                         sf_freq,
                         dated,
                         bondCalendar,
                         settlementDays,
                         bondDayCounter,
                         frequency,
                         rval,
                         faceAmount,
                         accrualConvention,
                         paymentConvention
                         ); 
    
    // show the results
    showCashFlows(bnd2);
    
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

    Date settle = Date(1, February, 2012);
    today = bondCalendar.advance(settle, -settlementDays, Days);
    
    Settings::instance().evaluationDate() = today;
    
    cout << "eval / settle " 
    << today << " / "
    << settle << endl;
    
    sf_bal.assign(sf_sch, sf_sch+13);
    SinkingFundBond bnd3(.0663,
                         Date(1, February, 2035),
                         sf_bal,
                         sf_freq,
                         Date(4, February, 2010),
                         bondCalendar,
                         settlementDays,
                         bondDayCounter,
                         frequency,
                         rval,
                         faceAmount,
                         accrualConvention,
                         paymentConvention
                         ); 
    
    showCashFlows(bnd3);
    
    cout << "cf yield: " << bnd3.toYield(109.36) << endl;
    
    return 0;
}

