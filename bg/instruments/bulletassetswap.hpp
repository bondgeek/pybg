/*
 *  bulletassetswap.hpp
 *  pybg
 *
 *  Created by BART MOSLEY on 9/2/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */
#ifndef FIXEDFLOATSWAP_H
#define FIXEDFLOATSWAP_H

#include <bg/instruments/fixedfloatswap.hpp>

namespace bondgeek {
        
    class BulletAssetSwap : public InstrumentBase, public AssetSwap  {
    protected:
        Real  _cleanPrice;
        /*        
        SwapPayType             _payerType; // payBondCoupon
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
         */
    public:
        BulletAssetSwap(const boost::shared_ptr< Bond > &bond,
                        boost::shared_ptr< IborIndex > iborIndex,
                        Real bondCleanPrice,
                        bool parAssetSwap=true,
                        bool payBondCoupon=false,
                        Spread spread=0.0,
                        bool parAssetSwap=true
                        // Eval date
                        Date eval_date=Date()
                        )
        : _cleanPrice(bondCleanPrice),
        InstrumentBase(eval_date), 
        AssetSwap(payBondCoupon,
                  bond,
                  bondCleanPrice,
                  iborIndex,
                  spread,
                  Schedule(),
                  DayCounter(),
                  parAssetSwap
                  )
        {}
        
        virtual void setEngine(CurveBase &crv, 
                               Real a=QL_EPSILON, 
                               Real sigma=QL_EPSILON,
                               bool lognormal=true) ;
    };
    
    void FixedFloatSwap::setEngine(CurveBase &crv, 
                                   Real a, 
                                   Real sigma,
                                   bool lognormal)   
    {
        typedef boost::shared_ptr<PricingEngine> PrcEnginePtr;
        
        PrcEnginePtr swapEngine = createPriceEngine<DiscountingSwapEngine>( \
                                            crv.discountingTermStructure()
                                            );
        
        setPricingEngine(swapEngine);
    }
#endif
