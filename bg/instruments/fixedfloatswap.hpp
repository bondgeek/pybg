/*
 *  fixedfloatswap.h
 *  bondgeek
 *
 *  Created by Bart Mosley on 5/18/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */
#ifndef FIXEDFLOATSWAP_H
#define FIXEDFLOATSWAP_H

#include <ql/quantlib.hpp>
#include <bg/instruments/instrumentbase.hpp>
#include <bg/indexbase.hpp>

namespace bondgeek {

    enum SwapPayType { 
        FixedReceiver = -1, 
        FixedPayer = 1 
        };

    class FixedFloatSwap : public InstrumentBase, public VanillaSwap {
    protected:
        SwapPayType             _payerType;
        Rate                    _fixedRate; // = 0.04;
        Spread                  _spread;
        Date                    _settlementDate; 
        Date                    _maturity;
        Real                    _notional; // = 1000000.0;
        
        // parameters
        Calendar _calendar;
        boost::shared_ptr<IborIndex> _iborIndex;

        // fixed leg
        Frequency               _fixedLegFrequency; 
        DayCounter              _fixedLegDayCounter; 
        BusinessDayConvention   _fixedLegConvention; 

        // floating leg
        Frequency                       _floatingLegFrequency; 
        DayCounter                      _floatingLegDayCounter; 
        BusinessDayConvention           _floatingLegConvention; 
        
        RelinkableHandle<YieldTermStructure> _discountingTermStructure;        
    public:
        FixedFloatSwap(Date settle, 
                       Date maturity, 
                       Rate fixedRate, 
                       boost::shared_ptr< IborIndex > iborIndex,
                       SwapPayType payerType=FixedPayer,
                       Spread floating_spread=0.0,
                       Real notional=1000000.0,
                       // Fixed Leg
                       Frequency fixedLegFrequency = Semiannual,
                       DayCounter fixedLegDayCounter = Thirty360(Thirty360::European),
                       BusinessDayConvention fixedLegConvention = ModifiedFollowing,
                       // floating leg 
                       Frequency floatingLegFrequency = Quarterly,
                       DayCounter floatingLegDayCounter = Actual360(),
                       BusinessDayConvention floatingLegConvention = ModifiedFollowing,
                       Calendar calendar = TARGET()
                       );

        VanillaSwap::Type paytype(SwapPayType paytype);
        
        virtual void setEngine(CurveBase &crv) ;
        virtual void setEngine(CurveBase &crv, 
                               Real &a, 
                               Real &sigma,
                               bool lognormal=true) ;    
        //Inspectors
        SwapPayType payerType(void) { return _payerType; }
        
    };

}
#endif
