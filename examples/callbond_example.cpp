/*
 *  callbond_test.cpp
 *  bondgeek
 *
 *  Created by BART MOSLEY on 6/3/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#include <bg/bondgeek.hpp>

#include <iostream>
#include <algorithm>

#include <boost/timer.hpp>

using namespace std;
using namespace QuantLib;
using namespace bondgeek;

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {
    Integer sessionId() { return 0; }
}
#endif

boost::shared_ptr<YieldTermStructure>
flatRate(const Date& today,
         const boost::shared_ptr<Quote>& forward,
         const DayCounter& dc,
         const Compounding& compounding,
         const Frequency& frequency) {
    return boost::shared_ptr<YieldTermStructure>(
                                                 new FlatForward(today,
                                                                 Handle<Quote>(forward),
                                                                 dc,
                                                                 compounding,
                                                                 frequency));
}


boost::shared_ptr<YieldTermStructure>
flatRate(const Date& today,
         Rate forward,
         const DayCounter& dc,
         const Compounding &compounding,
         const Frequency &frequency) {
    return flatRate(today,
                    boost::shared_ptr<Quote>(new SimpleQuote(forward)),
                    dc,
                    compounding,
                    frequency);
}

int main (int argc, char * const argv[]) 
{
    
    Date today = Date(16,October,2007);
    Settings::instance().evaluationDate() = today;
    
    cout <<  endl;
    cout << "Pricing a callable fixed rate bond using" << endl;
    cout << "Hull White model w/ reversion parameter = 0.03" << endl;
    cout << "BAC4.65 09/15/12  ISIN: US06060WBJ36" << endl;
    cout << "roughly five year tenor, ";
    cout << "quarterly coupon and call dates" << endl;
    cout << "reference date is : " << today << endl;
    cout << "3 day settle: " << TARGET().advance(today, 3, Days) << endl << endl;
    
    /* Bloomberg OAS1: "N" model (Hull White)
     varying volatility parameter
     
     The curve entered into Bloomberg OAS1 is a flat curve,
     at constant yield = 5.5%, semiannual compounding.
     Assume here OAS1 curve uses an ACT/ACT day counter,
     as documented in PFC1 as a "default" in the latter case.
     */
    
    // set up a flat curve corresponding to Bloomberg flat curve
    
    Rate bbCurveRate = 0.055;
    DayCounter bbDayCounter = ActualActual(ActualActual::Bond);
    InterestRate bbIR(bbCurveRate,bbDayCounter,Compounded,Semiannual);
    
    Handle<YieldTermStructure> termStructure(flatRate(today,
                                                      bbIR.rate(),
                                                      bbIR.dayCounter(),
                                                      bbIR.compounding(),
                                                      bbIR.frequency()));
    
    // set up the call schedule
    
    CallabilitySchedule callSchedule;
    Real callPrice = 100.;
    Size numberOfCallDates = 19;
    Date callDate = Date(15, December, 2007);
    
    Date sDate(callDate.serialNumber());
    for (Size i=0; i< numberOfCallDates; i++) {
        Calendar nullCalendar = NullCalendar();
        
        Callability::Price myPrice(callPrice,
                                   Callability::Price::Clean);
        callSchedule.push_back(
                               boost::shared_ptr<Callability>(
                                                              new Callability(myPrice,
                                                                              Callability::Call,
                                                                              sDate )));
        sDate = nullCalendar.advance(sDate, 3, Months);
    }
    
    cout << "calls: " << callDate << " to " << sDate << endl;
    
    // set up the callable bond
    
    Date dated = Date(16,September,2004);
    Date issue = dated;
    Date maturity = Date(15,September,2012);
    Natural settlementDays = 3;  // Bloomberg OAS1 settle is Oct 19, 2007
    Calendar bondCalendar = UnitedStates(UnitedStates::GovernmentBond);
    
    //Real coupon = .0465;
    Real coupon = .06;
    Frequency frequency = Quarterly;
    Real redemption = 100.0;
    Real faceAmount = 100.0;
    
    /* The 30/360 day counter Bloomberg uses for this bond cannot
     reproduce the US Bond/ISMA (constant) cashflows used in PFC1.
     Therefore use ActAct(Bond)
     */
    DayCounter bondDayCounter = ActualActual(ActualActual::Bond);
    
    // PFC1 shows no indication dates are being adjusted
    // for weekends/holidays for vanilla bonds
    BusinessDayConvention accrualConvention = Unadjusted;
    BusinessDayConvention paymentConvention = Unadjusted;
    
    Schedule sch(dated, maturity, Period(frequency), bondCalendar,
                 accrualConvention, accrualConvention,
                 DateGeneration::Backward, false);
    
    Size maxIterations = 1000;
    Real accuracy = 1e-8;
    Integer gridIntervals = 60;
    Real reversionParameter = .03;
    
    // output price/yield results for varying volatility parameter
    
    Real sigma = max(0.03, QL_EPSILON); // core dumps if zero on Cygwin
    
    boost::shared_ptr<ShortRateModel> hw0(
                                          new HullWhite(termStructure,reversionParameter,sigma));
    
    boost::shared_ptr<PricingEngine> engine0(
                                             new TreeCallableFixedRateBondEngine(hw0,gridIntervals));
    
    CallableFixedRateBond callableBond(settlementDays, faceAmount, sch,
                                       vector<Rate>(1, coupon),
                                       bondDayCounter, paymentConvention,
                                       redemption, issue, callSchedule);
    callableBond.setPricingEngine(engine0);
    
    cout << setprecision(5)
    << "Flat Rate: " << bbCurveRate << " | disc factor: " << 100.0*termStructure.currentLink()->discount(10.0) << endl;
    
    cout << setprecision(2)
    << showpoint
    << fixed
    << "sigma/vol (%) = "
    << 100.*sigma
    << ", mean reversion = " << reversionParameter 
    << endl;
    
    cout << "\nQuantLib price/yld (%)  ";
    cout << callableBond.cleanPrice() << " / "
    << 100. * callableBond.yield(bondDayCounter,
                                 Compounded,
                                 frequency,
                                 accuracy,
                                 maxIterations)
    << endl;

    cout << "\nNow price with a bg curve " << endl;
    string depotenors[] = {"1W", "1M", "3M", "6M", "9M", "1y"};
    double depospots[] = {.055, .055, .055, .055, .055, .055};
    string swaptenors[] = {"2y", "3y", "5y", "10y", "15y", "20y", "30y"};
    double swapspots[] = {.055, .055, .055, .055, .055, .055, .055};
    
    cout << "Test with new curve " << endl;
    RateHelperCurve acurve = RateHelperCurve(USDLiborCurve("3M"));
    acurve.update(depotenors, 
                  depospots, 
                  6,
                  swaptenors,                                                                      
                  swapspots,
                  7,
                  today);
    
    cout << "Curve: " 
    << acurve.discountingTermStructure().currentLink()->referenceDate() << "/"
    << acurve.discountingTermStructure().currentLink()->maxDate() << endl;
    
    cout << setprecision(3) 
    << "quote:  " << io::rate(acurve.tenorquote("10Y")) ;
    
    cout << setprecision(5)
    << " | disc factor: " << 100.0*acurve.discountingTermStructure().currentLink()->discount(10.0) << endl;
    
    boost::shared_ptr<ShortRateModel> hw1(
                                          new HullWhite(acurve.discountingTermStructure(), 
                                                        reversionParameter, 
                                                        sigma));
    
    boost::shared_ptr<PricingEngine> engine1(
                                             new TreeCallableFixedRateBondEngine(hw1,
                                                                                 gridIntervals));
    
    callableBond.setPricingEngine(engine1);
    
    
    cout << "price/yld (%)  ";
    cout << callableBond.cleanPrice() << " / "
    << 100. * callableBond.yield(bondDayCounter,
                                 Compounded,
                                 frequency,
                                 accuracy,
                                 maxIterations)
    << endl;
    
    
    cout << "\nbondgeek::CallBond\n";
    cout << setprecision(3)
    << "cpn: " << io::rate(coupon) 
    << " mty: " << maturity 
    << " call: " << callDate 
    << " @ " << callPrice 
    << endl;
    
    CallBond noncallbond(coupon, 
                         maturity,
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
    
    CallBond zeronc_bond(0.0, 
                         Date(15, February, 2023),
                         Date(29, June, 1995), 
                         bondCalendar, 
                         settlementDays,
                         Thirty360(Thirty360::BondBasis),
                         Semiannual, 
                         redemption,
                         faceAmount,
                         accrualConvention,
                         paymentConvention
                         );
    
    BulletBond bulletbond(coupon, 
                          maturity, 
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
    
    CallBond callbnd(coupon, 
                     maturity, 
                     callDate, 
                     callPrice, 
                     dated, 
                     bondCalendar, 
                     settlementDays,
                     bondDayCounter,
                     frequency,
                     frequency,
                     redemption,
                     faceAmount,
                     accrualConvention,
                     paymentConvention);
        
    cout << "Call Schedule" << endl;
    CallabilitySchedule  callbndsched1 = callableBond.callability();
    CallabilitySchedule  callbndsched2 = callbnd.callability();
    CallabilitySchedule  callbndsched3 = noncallbond.callability();
    
    vector< boost::shared_ptr<Callability> >::size_type sz1 = callbndsched1.size();
    vector< boost::shared_ptr<Callability> >::size_type sz2 = callbndsched2.size();
    vector< boost::shared_ptr<Callability> >::size_type sz3 = callbndsched3.size();
    
    cout << "Call sizes: " << endl <<
    "1) " << sz1 << endl <<
    "2) " << sz2 << endl <<
    "3) " << sz3 << endl;
    
    if (sz1 != sz2) {
        cout << "\nCall schedules not equal!!! " << sz1 << " vs " << sz2 << endl;
        for (int i=0; i<sz1; i++) {
            cout << callbndsched1[i]->date() << " | " << callbndsched1[i]->price().amount() << endl;
        }
        for (int i=0; i<sz2; i++) {
            cout << callbndsched2[i]->date() << " | " << callbndsched2[i]->price().amount() << endl;
        }
        return 1;
    } 
    
    callbnd.setPricingEngine(engine1);
    noncallbond.setPricingEngine(engine1);
    bulletbond.setPricingEngine(engine1);
    cout << "test value: " << callbnd.cleanPrice() << endl << endl;
    cout << "test value (noncall): " << noncallbond.cleanPrice() << endl << endl;
    cout << "test value (bullet): " << bulletbond.cleanPrice() << endl << endl;
    cout << "test value (bullet, toPrice): " << bulletbond.toPrice() << endl << endl;
    
    cout << "\n\nMuni Schedules " << endl;
    CallabilitySchedule muniSched = createBondCallSchedule(Date(15, December, 2007),
                                                           102.,
                                                           Date(15, December, 2009),
                                                           maturity
                                                           );
    
    CallBond callbnd_sched(coupon,
                           maturity,
                           muniSched,
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

    CallBond callbnd_sched1(coupon,
                           maturity,
                           Date(15, December, 2007),
                           102.,
                           Date(15, December, 2009),
                           dated, 
                           bondCalendar, 
                           settlementDays,
                           bondDayCounter,
                           frequency,
                           Annual,
                           redemption,
                           faceAmount,
                           accrualConvention,
                           paymentConvention
                           );
    
    
    CallabilitySchedule muniSched1 = callbnd_sched1.callability();
    vector< boost::shared_ptr<Callability> >::size_type sz_m = muniSched1.size();
    
    for (int i=0; i<sz_m; i++) 
    {
        cout << muniSched1[i]->date() << " | " << muniSched1[i]->price().amount() << endl;
    }
    
    callbnd_sched.setPricingEngine(engine1);
    callbnd_sched1.setPricingEngine(engine1);
    
    cout << "test value: " << callbnd_sched.toPrice() << " | " << callbnd_sched1.toPrice() << endl;
    cout << "yield to worst: " << callbnd_sched.toYield() << " | " << callbnd_sched1.toYield() << endl;
    cout << "price from ytw: " << callbnd_sched.toPrice(callbnd_sched.toYield()) 
    << " | " << callbnd_sched1.toPrice(callbnd_sched1.toYield()) << 
    endl << endl;
    
    //TODO: create vol/mean reversion matrix
    cout << "\nHull-White (normal) pricing " << endl;    
    Real sig[3] = {0.0, .01, .03};
    
    cout << "mean reversion = " << reversionParameter << endl;
    
    for (int i=0; i<3; i++) {
        callbnd.setEngine(acurve, reversionParameter, sig[i], false);
        zeronc_bond.setEngine(acurve, reversionParameter, sig[i], false);
        noncallbond.setEngine(acurve, reversionParameter, sig[i], false);
        bulletbond.setEngine(acurve);
        
        cout << setprecision(3) << "sigma: " << io::rate(sig[i]); 
        cout << setprecision(5) << " | value: " << callbnd.cleanPrice() ;
        cout << " | nc1: " << noncallbond.cleanPrice();
        cout << " bullet: " << bulletbond.cleanPrice();
        cout << " zero: " << zeronc_bond.cleanPrice() << "/"
        << 100. * zeronc_bond.yield(Thirty360(Thirty360::BondBasis),
                                    Compounded,
                                    Semiannual,
                                    accuracy,
                                    maxIterations);
        cout << endl;
    }
        
    cout << "\nLognormal pricing " << endl;
    
    Real mr = 0.0;
    Real v[3] = {0.0, .2017, .6763};
    
    cout << "mean reversion = " << mr  
    << endl;
        
    callbnd.oasEngine(acurve, mr, v[0], 0.0);
    
    for (int i=0; i<3; i++) {
        noncallbond.setEngine(acurve, mr, v[i]);
        bulletbond.setEngine(acurve);
        
        cout << setprecision(3) << "vol: " << io::rate(v[i]); 
        cout << setprecision(5) << " | value: " << callbnd.oasValue(0.0, v[i], mr);
        cout << " nc1: " << noncallbond.cleanPrice();
        cout << " bullet: " << bulletbond.cleanPrice();
        
    }
    
    Real testPx[] = {80., 85., 90., 95., 100., 100.25};
    
    cout << " vol: " << v[1] << endl << endl;
    
    boost::timer timer;
    
    for (int i=0; i<6; i++) {
        Real oasx = callbnd.oasT<Secant>(testPx[i], v[1]);
    
        cout << fixed << setprecision(5) 
        << "OAS: " << oasx ;
        cout << " > value: " << callbnd.oasValue(oasx) << endl;

    }
    Real t1 = timer.elapsed();
    cout << fixed << setprecision(3)
    << t1 << " s\n" << endl;
    Real t0=t1;

    for (int i=0; i<6; i++) {
        Real oasx = callbnd.oasT<Brent>(testPx[i], v[1]);
        
        cout << fixed << setprecision(5) 
        << "OAS: " << oasx ;
        cout << " > value: " << callbnd.oasValue(oasx) << endl;
        
    }
    
    t1  = timer.elapsed();
    cout << fixed << setprecision(3)
    << t1-t0 << " s\n" << endl;
    t0=t1;
    
    Real testspread = 0.01543;
    Real xvol = callbnd.oasImpliedVol(90., testspread);
    
    cout << "\nImpVol " << xvol 
    << " value " << callbnd.oasValue(testspread, xvol, 0.0, true)
    << endl ;
    
    
    t1  = timer.elapsed();
    cout << fixed << setprecision(3)
    << t1-t0 << " s\n" << endl;
        
    return 0;
}
