/*
 *  curvebase.cpp
 *  bondgeek
 *
 *  Created by BART MOSLEY on 6/4/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#include <bg/curvebase.hpp>

namespace bondgeek 
{	
	void CurveBase::build(const Date &todaysDate, const int &fixingDays) 
	{
		if (fixingDays >= 0) 
            _fixingDays = fixingDays;
		
		_todaysDate = todaysDate;
		
		if (_todaysDate == Date()) 
			_todaysDate = Settings::instance().evaluationDate();
		
		Settings::instance().evaluationDate() = _todaysDate;
		
		build();
	}
    
    boost::shared_ptr<YieldTermStructure>   CurveBase::spreadedTermStructurePtr(const Handle<Quote>& spread,
                                                                                Compounding comp,
                                                                                Frequency freq,
                                                                                const DayCounter& dc
                                                                                ) 
    {
        boost::shared_ptr<YieldTermStructure> zcrv(new ZeroSpreadedTermStructure(discountingTermStructure(), 
                                                                                 spread, 
                                                                                 comp,  
                                                                                 freq,  
                                                                                 dc
                                                                                 )
                                                   );
        return zcrv;
    }
	
}
