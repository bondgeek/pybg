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
	// output format for RHType
    std::ostream& operator<<(std::ostream& out, const RHType& timeunit) {
        switch (timeunit) {
            case DEPO:
                return out << "DepositRate";
            case FRA:
                return out << "FRA";
            case FUT:
                return out << "Futures";
            case SWAP:
                return out << "Swaps";
            default:
                QL_FAIL("unknown TimeUnit");
        }
    } 
	
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
	
}
