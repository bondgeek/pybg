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
    Date CurveBase::curveDate()
    {
        return Settings::instance().evaluationDate();
    }
    
    Date CurveBase::setCurveDate(Date todays_date) 
    {
        Date date_ref;
        
        if (todays_date == Date()) 
        {
            date_ref = Settings::instance().evaluationDate();
        }
        else {
            // Use fixing calendar to avoid problems
            date_ref = this->calendar().adjust(todays_date);
            
            Settings::instance().evaluationDate() = date_ref;
        }
        
        return date_ref;
    }
    
    Date CurveBase::advanceCurveDate(int n, TimeUnit unit)
    {
        Calendar cal = this->calendar();
        BusinessDayConvention bdconv = _swFloatingLegIndex->businessDayConvention();
        
        Date date_ref = Settings::instance().evaluationDate();
        
        date_ref = cal.advance(date_ref, n, unit, bdconv);
        
        return this->setCurveDate(date_ref);
    }
    
    void CurveBase::build(const Date &todays_date) 
    {
        
        this->setCurveDate(todays_date);
        
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
