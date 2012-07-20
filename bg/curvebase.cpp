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
    Date CurveBase::setEvalDate(Date todays_date, const int &fixing_days) 
    {
        if (fixing_days >= 0) 
            _fixingDays = fixing_days;
        
        if (todays_date == Date()) 
        {
            _todaysDate = Settings::instance().evaluationDate();
        }
        else {
            // Date date_ref = this->_calendar.adjust(todaysDate);
            // Use fixing calendar to avoid problems
            Date date_ref = this->fixingCalendar().adjust(todays_date);
            _todaysDate = date_ref;
        }
        
        Settings::instance().evaluationDate() = _todaysDate;
        
        return _todaysDate;
    }
    
    void CurveBase::build(const Date &todays_date, const int &fixing_days) 
    {
        
        this->setEvalDate(todays_date, fixing_days);
        
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
    
    Real CurveBase::discount(double years, bool extrapolate)
    { 
        Real df = this->_yieldTermStructure->discount(years, extrapolate);
        return df; 
    }
    
    Real CurveBase::discount(const Date &date, bool extrapolate)
    { 
        Real df = this->_yieldTermStructure->discount(date, extrapolate);
        return df; 
    }

}
