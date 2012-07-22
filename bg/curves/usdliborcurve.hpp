/*
 *  usdliborcurve.hpp
 *  pybg
 *
 *  Created by Bart Mosley on 6/22/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */
#ifndef USDLIBORCURVE_HPP
#define USDLIBORCURVE_HPP

#include <bg/curvebase.hpp>
#include <bg/date_utilities.hpp>

using namespace QuantLib;

namespace bondgeek {
    class USDLiborCurve : public CurveBase {
    protected:
    public:
        USDLiborCurve():
        CurveBase(boost::shared_ptr<IborIndex>(new USDLibor(Period(3,Months))),
                  2,
                  Semiannual,
                  ModifiedFollowing,
                  Thirty360(Thirty360::European),
                  ActualActual(ActualActual::ISDA)
                  )
        {}
        
        USDLiborCurve(string tenor, Frequency fixedFrequency=Semiannual):
        CurveBase(boost::shared_ptr<IborIndex>(new USDLibor( Tenor(tenor) )),
                  2,
                  fixedFrequency,
                  ModifiedFollowing,
                  Thirty360(Thirty360::European),
                  ActualActual(ActualActual::ISDA)
                  )
        {}
    };

}

#endif