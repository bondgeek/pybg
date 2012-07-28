/*
 *  InstrumentBase.hpp
 *  bondgeek
 *
 *  Created by Bart Mosley on 6/1/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#ifndef INSTRUMENTBASE_H
#define INSTRUMENTBASE_H

#include <ql/quantlib.hpp>

#include <bg/curves/ratehelpercurve.hpp>

using namespace QuantLib;

namespace bondgeek 
{

    class InstrumentBase {
    protected:
        InstrumentBase() {};
        
    public:
        // virtual function 
        // defined at instrument level 
        virtual void setEngine(CurveBase &crv) =0 ;
        virtual void setEngine(CurveBase &crv, 
                               Real &parm,
                               Real &sigma,
                               bool lognormal=true) =0 ;
    };
}
#endif