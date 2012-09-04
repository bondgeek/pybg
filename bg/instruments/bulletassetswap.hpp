/*
 *  bulletassetswap.hpp
 *  pybg
 *
 *  Created by BART MOSLEY on 9/2/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */
#ifndef BULLETASSETSWAP_H
#define BULLETASSETSWAP_H

#include <ql/quantlib.hpp>
#include <bg/instruments/instrumentbase.hpp>
#include <bg/indexbase.hpp>

namespace bondgeek {
        
    class BulletAssetSwap : public InstrumentBase, public AssetSwap  {
    protected:
        Real  _cleanPrice;
        
    public:
        BulletAssetSwap(const boost::shared_ptr< Bond > &bond,
                        boost::shared_ptr< IborIndex > iborIndex,
                        Real bondCleanPrice,
                        bool parAssetSwap=true,
                        bool payBondCoupon=false,
                        Spread spread=0.0,
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
    
    void BulletAssetSwap::setEngine(CurveBase &crv, 
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
}
#endif
