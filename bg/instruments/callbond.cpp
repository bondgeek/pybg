/*
 *  callbond.cpp
 *  bondgeek
 *
 *  Created by BART MOSLEY on 6/16/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#include <bg/instruments/callbond.hpp>

namespace bondgeek {
    // Callable Bonds
    CallabilitySchedule createBondCallSchedule(const Date &firstCall,
                                               const Date &endDate, 
                                               const Real &callPrice,
                                               const Frequency &callFrequency
                                               ) 
    // fixed call price
    // TODO:  input list of dates, prices
    // TODO:  classic muni schedule, first premium date/price, first par date/price
    {
        CallabilitySchedule callSched;
        Calendar nullCalendar = NullCalendar();
        
        Date sdate(firstCall.serialNumber());
        while (ActualActual().dayCount(sdate, endDate) > 0) 
        {
            Callability::Price callPx(callPrice, Callability::Price::Clean);
            
            callSched.push_back(boost::shared_ptr<Callability>(
                                                               new Callability(callPx,
                                                                               Callability::Call,
                                                                               sdate)));
            
            sdate = nullCalendar.advance(sdate, Period(callFrequency), Unadjusted, false);
        }
        
        return callSched;
        
    }
    
    CallBond::CallBond(const Rate &coupon,
                       const Date &maturity,
                       const Date &callDate,
                       const Real &callPrice, 
                       const Date &issue_date,
                       Calendar calendar,
                       Natural settlementDays,
                       DayCounter daycounter,
                       Frequency payFrequency,
                       Frequency callFrequency,
                       Real redemption,
                       Real faceamount,
                       BusinessDayConvention accrualConvention,
                       BusinessDayConvention paymentConvention
                       ) : 
    _coupon(coupon),
    _maturity(maturity),
    _issue_date(issue_date),
    _calendar(calendar),
    _settlementDays(settlementDays),
    _daycounter(daycounter),
    _payfrequency(payFrequency),
    _redemption(redemption),
    _faceamount(faceamount),
    _accrualConvention(accrualConvention),
    _paymentConvention(paymentConvention),
    CallableFixedRateBond(settlementDays, 
                          faceamount, 
                          Schedule(issue_date,
                                   maturity, 
                                   Period(payFrequency),
                                   calendar,
                                   accrualConvention, 
                                   accrualConvention, 
                                   DateGeneration::Backward, 
                                   false),
                          std::vector<Rate>(1, coupon),                              
                          daycounter, 
                          paymentConvention, 
                          redemption, 
                          issue_date, 
                          createBondCallSchedule(callDate, 
                                                 maturity, 
                                                 callPrice, 
                                                 callFrequency)
                          )
    {}
    
    CallBond::CallBond(const Rate &coupon,
                       const Date &maturity,
                       const Date &issue_date,
                       Calendar calendar,
                       Natural settlementDays,
                       DayCounter daycounter,
                       Frequency payFrequency,
                       Real redemption,
                       Real faceamount,
                       BusinessDayConvention accrualConvention,
                       BusinessDayConvention paymentConvention
                       ) : 
    _coupon(coupon),
    _maturity(maturity),
    _issue_date(issue_date),
    _calendar(calendar),
    _settlementDays(settlementDays),
    _daycounter(daycounter),
    _payfrequency(payFrequency),
    _redemption(redemption),
    _faceamount(faceamount),
    _accrualConvention(accrualConvention),
    _paymentConvention(paymentConvention),
    CallableFixedRateBond(settlementDays, 
                          faceamount, 
                          Schedule(issue_date,
                                   maturity, 
                                   Period(payFrequency),
                                   calendar,
                                   accrualConvention, 
                                   accrualConvention, 
                                   DateGeneration::Backward, 
                                   false),
                          std::vector<Rate>(1, coupon),                              
                          daycounter, 
                          paymentConvention, 
                          redemption, 
                          issue_date, 
                          CallabilitySchedule()
                          )
    {}
    
    
    void CallBond::setEngine(CurveBase &crv, 
                             Real &a, 
                             Real &sigma,
                             bool lognormal)  
    {
        a = max(QL_EPSILON, a);
        sigma = max(QL_EPSILON, sigma);
        
        boost::shared_ptr<PricingEngine> pxEngine = lognormal ? 
        createOptionEngine<BlackKarasinski, TreeCallableFixedRateBondEngine>(crv.discountingTermStructure(), a, sigma) : 
        createOptionEngine<HullWhite, TreeCallableFixedRateBondEngine>(crv.discountingTermStructure(), a, sigma);
        
        setPricingEngine(pxEngine);
    }
    
    void CallBond::setEngine(CurveBase &crv) 
    {
        Real x = QL_EPSILON;
        setEngine(crv, x, x, true);
    }
    
    // OAS Functions
    void    CallBond::oasEngine(CurveBase &crv, 
                                Real &a, 
                                Real &sigma,
                                const Real &spread,
                                bool lognormal
                                )
    {
        if ( _spreadedTermStructure.empty() ) 
        {
            _curveSpread = boost::shared_ptr<SimpleQuote>(new SimpleQuote(spread));
            
            _spreadedTermStructure.linkTo(
                                          crv.spreadedTermStructurePtr(Handle<Quote>(_curveSpread),
                                                                       Compounded,
                                                                       _payfrequency,
                                                                       _daycounter)
                                          );
        }
        
        oasEngine(a, sigma, spread, lognormal);
        
    }
    
    void    CallBond::oasEngine(Real &a, 
                                Real &sigma,
                                const Real &spread,
                                bool lognormal
                                )
    {
        QL_REQUIRE(!_spreadedTermStructure.empty(),
                   "spreadedTermStructure not set before call to oasEngine(a, sigma, spread)");
        
        setSpread(spread);
        
        boost::shared_ptr<PricingEngine> prxEngine = lognormal ?
        createOptionEngine<BlackKarasinski, TreeCallableFixedRateBondEngine>(_spreadedTermStructure, a, sigma) :
        createOptionEngine<HullWhite, TreeCallableFixedRateBondEngine>(_spreadedTermStructure, a, sigma);
        
        setPricingEngine(prxEngine);
    }
    
    void CallBond::setSpread(const Real &spread)
    {
        if (!_curveSpread->isValid() )
            _curveSpread = boost::shared_ptr<SimpleQuote>(new SimpleQuote(0.0));
        
        _curveSpread->setValue(spread);
    }

    Real CallBond::getSpread()
    {
        if (!_curveSpread->isValid() )
            return 0.0;
        
        return _curveSpread->value();
    }
    
    // OAS Functions
    Real CallBond::oasValue(const Real &spread)
    {
        setSpread(spread);
        return cleanPrice();
    }
    
    Real CallBond::oasValue(const Real &spread, Real &sigma, Real a, bool lognormal)
    {
        oasEngine(a, sigma, spread, lognormal);
        return cleanPrice();
    }
    
    Real CallBond::oas(const Real &bondprice,
                       Real sigma, 
                       Real a,
                       bool lognormal
                       )
    {
        Real step = 0.001;
        Real accuracy=1.0e-7;
        Real guess = 0.0;
        
        return oasT<Brent>(bondprice, sigma, a, lognormal, step, accuracy, guess);
    }
    
    Real CallBond::oasImpliedVol(const Real &bondprice, 
                                 Real spread,
                                 Real a,
                                 bool lognormal
                                 )
    {
        Real accuracy=1.0e-7;
        Real guess = 0.01;
        Real step = 0.001;
        
        return oasImpliedVolT<Brent>(bondprice, spread, a, lognormal, step, accuracy, guess);            
        
    }
    
}

