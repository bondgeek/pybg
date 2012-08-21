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
    
    CallabilitySchedule createBondCallSchedule(const Date &firstCall,
                                               const Real &callPrice,
                                               const Date &parCall, 
                                               const Date &endDate,
                                               const Frequency &callFrequency,
                                               const Real &faceAmount,
                                               const DayCounter &dayCounter
                                               ) 
    // classic muni schedule, first premium date/price, first par date/price
    {
        CallabilitySchedule callSched;
        Calendar nullCalendar = NullCalendar();
        
        QL_REQUIRE(firstCall <= parCall, "first call must be less than " << 
                   " or equal par call");

        Real dtp = 0.0;
        int frequency = callFrequency;
        
        if (callPrice > faceAmount && parCall > firstCall) {
            Real callyears = dayCounter.yearFraction(firstCall, parCall);
            
            dtp = (callPrice-faceAmount)/(frequency*callyears);
            
        }
        
        Date fdate(firstCall.serialNumber());
        Real cPrice = callPrice;
        while (ActualActual().dayCount(fdate, endDate) > 0) 
        {
            
            Callability::Price callPx(cPrice, Callability::Price::Clean);
            
            callSched.push_back(boost::shared_ptr<Callability>(
                                                               new Callability(callPx,
                                                                               Callability::Call,
                                                                               fdate)));
            
            fdate = nullCalendar.advance(fdate, Period(callFrequency), Unadjusted, false);
            
            if (fdate >= parCall) {
                cPrice = faceAmount;
            }
            else {
                cPrice -= dtp;
            }
        }
        
        return callSched;
        
    }
    
    // Constructors
    // Call Schedule
    CallBond::CallBond(const Rate &coupon,
                       const Date &maturity,
                       const CallabilitySchedule &callSched, 
                       const Date &issue_date,
                       Calendar calendar,
                       Natural settlementDays,
                       DayCounter daycounter,
                       Frequency payfrequency,
                       Real redemption,
                       Real faceamount,
                       BusinessDayConvention accrualConvention,
                       BusinessDayConvention paymentConvention,
                       Date eval_date
                       ) : 
    _coupon(coupon),
    _maturity(maturity),
    _issue_date(issue_date),
    BondBase(calendar,
             settlementDays,
             daycounter,
             payfrequency,
             redemption,
             faceamount,
             accrualConvention,
             paymentConvention,
             eval_date),
    CallableFixedRateBond(settlementDays, 
                          faceamount, 
                          Schedule(issue_date,
                                   maturity, 
                                   Period(payfrequency),
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
                          callSched
                          )
    {}
    
    //Fixed Call Price
    CallBond::CallBond(const Rate &coupon,
                       const Date &maturity,
                       const Date &callDate,
                       const Real &callPrice, 
                       const Date &issue_date,
                       Calendar calendar,
                       Natural settlementDays,
                       DayCounter daycounter,
                       Frequency payfrequency,
                       Frequency callFrequency,
                       Real redemption,
                       Real faceamount,
                       BusinessDayConvention accrualConvention,
                       BusinessDayConvention paymentConvention,
                       Date eval_date
                       ) : 
    _coupon(coupon),
    _maturity(maturity),
    _issue_date(issue_date),
    BondBase(calendar,
             settlementDays,
             daycounter,
             payfrequency,
             redemption,
             faceamount,
             accrualConvention,
             paymentConvention,
             eval_date),
    CallableFixedRateBond(settlementDays, 
                          faceamount, 
                          Schedule(issue_date,
                                   maturity, 
                                   Period(payfrequency),
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
    
    //Declining Call Schedule
    CallBond::CallBond(const Rate &coupon,
                       const Date &maturity,
                       const Date &callDate,
                       const Real &callPrice, 
                       const Date &parCallDate,
                       const Date &issue_date,
                       Calendar calendar,
                       Natural settlementDays,
                       DayCounter daycounter,
                       Frequency payfrequency,
                       Frequency callFrequency,
                       Real redemption,
                       Real faceamount,
                       BusinessDayConvention accrualConvention,
                       BusinessDayConvention paymentConvention,
                       Date eval_date
                       ) : 
    _coupon(coupon),
    _maturity(maturity),
    _issue_date(issue_date),
    BondBase(calendar,
             settlementDays,
             daycounter,
             payfrequency,
             redemption,
             faceamount,
             accrualConvention,
             paymentConvention,
             eval_date),
    CallableFixedRateBond(settlementDays, 
                          faceamount, 
                          Schedule(issue_date,
                                   maturity, 
                                   Period(payfrequency),
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
                                                 callPrice,
                                                 parCallDate,
                                                 maturity,  
                                                 callFrequency,
                                                 faceamount)
                          )
    {}
    
    // No Call Schedule, Non-Call
    CallBond::CallBond(const Rate &coupon,
                       const Date &maturity,
                       const Date &issue_date,
                       Calendar calendar,
                       Natural settlementDays,
                       DayCounter daycounter,
                       Frequency payfrequency,
                       Real redemption,
                       Real faceamount,
                       BusinessDayConvention accrualConvention,
                       BusinessDayConvention paymentConvention,
                       Date eval_date
                       ) : 
    _coupon(coupon),
    _maturity(maturity),
    _issue_date(issue_date),
    BondBase(calendar,
             settlementDays,
             daycounter,
             payfrequency,
             redemption,
             faceamount,
             accrualConvention,
             paymentConvention,
             eval_date),
    CallableFixedRateBond(settlementDays, 
                          faceamount, 
                          Schedule(issue_date,
                                   maturity, 
                                   Period(payfrequency),
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
                             Real a, 
                             Real sigma,
                             bool lognormal)  
    {
        a = max(QL_EPSILON, a);
        sigma = max(QL_EPSILON, sigma);
        
        boost::shared_ptr<PricingEngine> pxEngine = lognormal ? 
        createOptionEngine<BlackKarasinski, TreeCallableFixedRateBondEngine>(crv.discountingTermStructure(), a, sigma) : 
        createOptionEngine<HullWhite, TreeCallableFixedRateBondEngine>(crv.discountingTermStructure(), a, sigma);
        
        setPricingEngine(pxEngine);
    }

    // Bond Math Functions  
    BulletBond CallBond::bullet(Real redemption)
    {
        BulletBond newbond(_coupon,
                           _maturity,
                           _issue_date,
                           _calendar,
                           _settlementDays,
                           _daycounter,
                           _payfrequency,
                           redemption,
                           _faceamount,
                           _accrualConvention,
                           _paymentConvention);
        return newbond;
    }
    
    BulletBond CallBond::bullet(Date &redemptionDate, Real &redemptionPrice,
                                Real coupon)
    {
        if (coupon < 0.0) {
            coupon = _coupon;
        }
        
        BulletBond newbond(coupon,
                           redemptionDate,
                           _issue_date,
                           _calendar,
                           _settlementDays,
                           _daycounter,
                           _payfrequency,
                           redemptionPrice,
                           _faceamount,
                           _accrualConvention,
                           _paymentConvention);
        return newbond;
    }
    
    double CallBond::toPrice(Rate bondyield)
    {
        // iterate thru call schedule for all declining call prices
        // (yield to worst date will be shortest date with the same price)
        // to determine if lowest price based on yield
        
        CallabilitySchedule  calls = this->callability();
        vector< boost::shared_ptr<Callability> >::size_type sz = calls.size();
        
        Real price = this->ytmToPrice(bondyield);
        _ytwFeature = -1;
        
        Real newPrice;
        
        Date callDate;
        Real prvCallPx = 0.0;
        Real newCallPx;
        
        if (price > _faceamount) 
        {
            for (unsigned int i = 0; i < sz; i++) 
            {
                newCallPx = calls[i]->price().amount();
                if (newCallPx < prvCallPx) {
                    
                    callDate = calls[i]->date();
                    BulletBond callbond = bullet(callDate, 
                                                 newCallPx
                                                 );
                    
                    newPrice = callbond.toPrice(bondyield);
                    if (newPrice < price)
                    {
                        price = newPrice;
                        _ytwFeature = i;
                    }
                }
                
            }
        }
        
        return price;
    }
    
    double CallBond::toYield(Real bondprice)
    {
        CallabilitySchedule  calls = this->callability();
        vector< boost::shared_ptr<Callability> >::size_type sz = calls.size();
        
        Real ytw = this->toYTM(bondprice);
        _ytwFeature = -1;
        
        Real newYield;
        
        Date callDate;
        Real prvCallPx = 0.0;
        Real newCallPx;
        
        if (bondprice > _faceamount) 
        {
            for (unsigned int i = 0; i < sz; i++) 
            {
                newCallPx = calls[i]->price().amount();
                
                if (newCallPx < prvCallPx) 
                {
                    callDate = calls[i]->date();
                    BulletBond callbond = bullet(callDate, 
                                                 newCallPx
                                                 );
                    
                    newYield = callbond.toYield(bondprice);
                    if (newYield < ytw)
                    {
                        ytw = newYield;
                        _ytwFeature = i;
                    }
                }
                
            }
        }
        
        return ytw;
    }
    
    double CallBond::toYTM()
    {
        return this->toYTM(this->cleanPrice(), _redemption);
    }
    
    double CallBond::toYTM(Real bondprice, Real redemption)
    {        
        BulletBond mtyBond = bullet(redemption);
        
        return mtyBond.toYield(bondprice);
    }
    
    double CallBond::ytmToPrice(Real bondyield)
    {        
        BulletBond mtyBond = bullet();
        
        return mtyBond.toPrice(bondyield);
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

