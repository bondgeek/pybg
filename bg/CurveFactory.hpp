/*
 *  CurveFactory.hpp
 *  bondgeek
 *
 *  Created by BART MOSLEY on 6/18/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */
#ifndef CURVEFACTORY_H
#define CURVEFACTORY_H

#include <string>

#include <bg/date_utilities.hpp>
#include <bg/repository.hpp>

#include <bg/indexbase.hpp>
#include <bg/curves/curves.hpp>

#include <bg/instruments/bulletbond.hpp>
#include <bg/instruments/callbond.hpp>

#include <bg/swaptype.hpp>
#include <bg/bondtype.hpp>

namespace bondgeek {
    
    // TODO: Not a Model element, should probably be in python-based Controller 
    class CurveFactory : public QuantLib::Singleton<CurveFactory> {
        friend class QuantLib::Singleton<CurveFactory>;
        
        Repository<std::string, CurveBase> _curveTypes; 
        
    private:
        CurveFactory() {
            init_curvebases();
        }
        
    public:
        void init_curvebases(void);
        void init_curvebases(std::string key, CurveBase crvtype);
        
        const CurveBase& curveType(const std::string &key) {return _curveTypes[key];}
        
        // Curve creation 
        RateHelperCurve ratehelperCurve(std::string key);        
        RateHelperCurve ratehelperCurve(std::string key, 
                                        Date todaysDate,
                                        std::string depotenors[],
                                        double depospots[],
                                        int depocount,
                                        std::string swaptenors[],
                                        double swapspots[],
                                        int swapcount,
                                        int fixingDays = -1);        
    };

}
#endif