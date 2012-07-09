/*
 *   euriborcurve.hpp
 *  pybg
 *
 *  Created by Bart Mosley on 7/2/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */
#ifndef EURIBORCURVE_HPP
#define EURIBORCURVE_HPP

#include <bg/curvebase.hpp>
#include <bg/date_utilities.hpp>

using namespace QuantLib;

namespace bondgeek {
    class EURiborCurve : public CurveBase {
    protected:
    public:
        EURiborCurve():
        CurveBase(boost::shared_ptr<IborIndex>(new Euribor(Period(6, Months))),
                  TARGET(),
                  2,
                  Actual360(),
                  Annual,
                  Unadjusted,
                  Thirty360(Thirty360::European),
                  ActualActual(ActualActual::ISDA)
                  )
        {}
        
        EURiborCurve(string tenor, Frequency fixedFrequency=Annual):
        CurveBase(boost::shared_ptr<IborIndex>(new Euribor( Tenor(tenor) )),
                  TARGET(),
                  2,
                  Actual360(),
                  fixedFrequency,
                  Unadjusted,
                  Thirty360(Thirty360::European),
                  ActualActual(ActualActual::ISDA)
                  )
        {
        }
    };
    
}

#endif