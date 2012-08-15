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
        
        InstrumentBase(){}

    public:        
        
        InstrumentBase(Date eval_date) { this->set_eval_date(eval_date); }
        
        // Inspectors
        Date    get_eval_date() { return get_evaluation_date(); }
        void    set_eval_date(Date &date_input) 
        {
            if (date_input != Date())
                set_evaluation_date(date_input);
        }

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