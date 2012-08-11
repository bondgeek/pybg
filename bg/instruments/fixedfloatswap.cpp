/*
 *  fxflswap.cpp
 *  bondgeek
 *
 *  Created by Bart Mosley on 5/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include <bg/instruments/fixedfloatswap.hpp>

namespace bondgeek {
    
    FixedFloatSwap::FixedFloatSwap(Date settle,  
                                   Date maturity, 
                                   Rate fixedRate, 
                                   boost::shared_ptr< IborIndex > iborIndex,
                                   SwapPayType payerType,
                                   Spread floating_spread,
                                   Real notional,
                                   
                                   // Fixed Leg
                                   Frequency fixedLegFrequency,
                                   DayCounter fixedLegDayCounter,
                                   BusinessDayConvention fixedLegConvention,
                                   
                                   // floating leg 
                                   Frequency floatingLegFrequency,
                                   DayCounter floatingLegDayCounter,
                                   BusinessDayConvention floatingLegConvention,
                                   Calendar calendar,

                                   Date eval_date
                                   ) : 
    _settlementDate(settle),
    _maturity(maturity),
    _payerType(payerType),  
    _fixedRate(fixedRate),
    _spread(floating_spread),
    _notional(notional),
    _fixedLegFrequency(fixedLegFrequency),
    _fixedLegDayCounter(fixedLegDayCounter),
    _fixedLegConvention(fixedLegConvention),
    _floatingLegFrequency(floatingLegFrequency),
    _floatingLegDayCounter(floatingLegDayCounter),
    _floatingLegConvention(floatingLegConvention),
    _calendar(calendar),
    InstrumentBase(eval_date),
    VanillaSwap(paytype(payerType), 
                notional,
                Schedule(settle, 
                         maturity,
                         Period(fixedLegFrequency),
                         calendar, 
                         fixedLegConvention,
                         fixedLegConvention,
                         DateGeneration::Forward, 
                         false), 
                fixedRate, 
                fixedLegDayCounter,
                Schedule(settle, 
                         maturity,
                         Period(floatingLegFrequency),
                         calendar, 
                         floatingLegConvention,
                         floatingLegConvention,
                         DateGeneration::Forward, 
                         false), 
                iborIndex, 
                floating_spread,
                floatingLegDayCounter) 
    {}
    
    
    VanillaSwap::Type FixedFloatSwap::paytype(SwapPayType paytype) 
    {
        switch (paytype) {
            case FixedPayer:
                return VanillaSwap::Payer;
                
            case FixedReceiver:
                return VanillaSwap::Receiver;
                
            default:
                return VanillaSwap::Payer;
                
        }
    }
    
    void FixedFloatSwap::setEngine(boost::shared_ptr<CurveBase> crv)  
    {
        typedef boost::shared_ptr<PricingEngine> PrcEnginePtr;
        
        PrcEnginePtr swapEngine = createPriceEngine<DiscountingSwapEngine>( \
                                                crv->discountingTermStructure()
                                                );
        
        setPricingEngine(swapEngine);
    }

    void FixedFloatSwap::setEngine(CurveBase &crv)  
    {
        typedef boost::shared_ptr<PricingEngine> PrcEnginePtr;
        
        PrcEnginePtr swapEngine = createPriceEngine<DiscountingSwapEngine>( \
                                                crv.discountingTermStructure()
                                                );
        
        setPricingEngine(swapEngine);
    }
    
    void FixedFloatSwap::setEngine(CurveBase &crv, 
                                   Real &a, 
                                   Real &sigma,
                                   bool lognormal)  
    {
        setEngine(crv);
    }
    
}