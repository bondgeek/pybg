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
        init_curvebases("EURANN_6M", EURiborCurve("6M"));
                        
        init_curvebases("USDSEMI_QTR", USDLiborCurve("3M", Semiannual));
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
        
        RateHelperCurve rhcurve( curveType(key) );
        
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
