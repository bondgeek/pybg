/*
 *  CurveFactory.cpp
 *  bondgeek
 *
 *  Created by BART MOSLEY on 6/18/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#include <bg/curvefactory.hpp>

namespace bondgeek 
{
    void CurveFactory::init_curvebases(void) {
        init_curvebases("default", CurveBase());
        init_curvebases("EURANN_6M", CurveBase(TARGET(),
                                               2,
                                               Actual360(),
                                               boost::shared_ptr<IborIndex>(new Euribor6M),
                                               Annual,
                                               Unadjusted,
                                               Thirty360(Thirty360::European),
                                               ActualActual(ActualActual::ISDA)
											   ) 
						);
        init_curvebases("USDSEMI_QTR", CurveBase(TARGET(),
                                                 2,
                                                 Actual360(),
                                                 boost::shared_ptr<IborIndex>(new USDLibor(Period(3,Months))),
                                                 Semiannual,
                                                 ModifiedFollowing,
                                                 Thirty360(Thirty360::European),
                                                 ActualActual(ActualActual::ISDA)
												 ) 
						);
    }
    
    void CurveFactory::init_curvebases(std::string key, CurveBase crvtype) {
        _curveTypes.add_item(key, crvtype);
    }
    
    // Curve creation
    RateHelperCurve CurveFactory::ratehelperCurve(std::string key) 
    {
        RateHelperCurve swapcrv(curveType(key));
        return swapcrv;
    }
    
    RateHelperCurve CurveFactory::ratehelperCurve(std::string key, 
                                                  Date todaysDate,
                                                  std::string depotenors[],
                                                  double depospots[],
                                                  int depocount,                          
                                                  std::string swaptenors[],
                                                  double swapspots[],
                                                  int swapcount,
                                                  int fixingDays) 
    {
        CurveMap depocurve;
        CurveMap swapcurve;
        RateHelperCurve rhcurve(curveType(key));
        
        for (int i=0; i<depocount; i++) 
            depocurve[depotenors[i]] = depospots[i];
        
        for (int i=0; i<swapcount; i++) 
            swapcurve[swaptenors[i]] = swapspots[i];
        
        rhcurve.add_depos(depocurve);
        rhcurve.add_swaps(swapcurve);
        
        rhcurve.build(todaysDate, fixingDays);
        
        return rhcurve;
    }
}
