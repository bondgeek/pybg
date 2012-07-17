/*
 *  bginstrument.hpp
 *  bondgeek
 *
 *  Created by Bart Mosley on 6/1/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#ifndef BGINSTRUMENT_H
#define BGINSTRUMENT_H

#include <ql/quantlib.hpp>

#include <bg/curves/ratehelpercurve.hpp>

using namespace QuantLib;

namespace bondgeek 
{

    class BGInstrument {
    protected:
        BGInstrument() {};
        
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