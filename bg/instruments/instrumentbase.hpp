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
        Date    _eval_date;

        InstrumentBase() { 
            _eval_date = Settings::instance().evaluationDate();
            cout << "ib: " << _eval_date << endl;
            cout << "settings: " << &Settings::instance() << endl;
        };
        
    public:
        // Inspectors
        Date    get_eval_date() { return _eval_date; }

        // virtual function 
        // defined at instrument level 
        virtual void setEngine(boost::shared_ptr<CurveBase> crvptr) =0;
        virtual void setEngine(CurveBase &crv) =0 ;
        virtual void setEngine(CurveBase &crv, 
                               Real &parm,
                               Real &sigma,
                               bool lognormal=true) =0 ;
    };
}
#endif