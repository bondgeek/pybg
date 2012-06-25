/*
 *  RateHelperCurve.cpp
 *
 *  Created by Bart Mosley on 5/18/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#include <bg/curves/ratehelpercurve.hpp>

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
                QL_FAIL("unknown RateHelperType");
        }
    } 
    
	boost::shared_ptr<RateHelper> RateHelperCurve::newRateHelper(const Period &tnr, 
                                                                 const Rate &quote, 
                                                                 const RHType &rhtype
                                                                 ) 
	{
		boost::shared_ptr<Quote> _quote(new SimpleQuote(quote));
		return newRateHelper(tnr, _quote, rhtype);
	}
	
	boost::shared_ptr<RateHelper> RateHelperCurve::newRateHelper(const Period &tnr, 
                                                                 const boost::shared_ptr<Quote> &_quote, 
                                                                 const RHType &rhtype
                                                                 ) 
	{
		switch(rhtype) {
			case DEPO:
				return boost::shared_ptr<RateHelper>(new DepositRateHelper(
																		   Handle<Quote>(_quote),
																		   tnr, 
																		   _fixingDays,
																		   _calendar, 
																		   ModifiedFollowing,
																		   true, 
																		   _depositDayCounter)) ;
				break;
			
            case SWAP:
				return boost::shared_ptr<RateHelper>(new SwapRateHelper(
																		Handle<Quote>(_quote), 
																		tnr,
																		_calendar, 
																		_swFixedLegFrequency,
																		_swFixedLegConvention, 
																		_swFixedLegDayCounter,
																		_swFloatingLegIndex));
				break;
            
			default:
				break;
		}
		return boost::shared_ptr<RateHelper>();
	} 
	
	void RateHelperCurve::add_ratehelpers(CurveMap crv, RHType type) 
	{
		CurveMap::iterator it;
		boost::shared_ptr<SimpleQuote> quote;
		Period tnr;

		for ( it=crv.begin() ; it != crv.end(); it++ ){
			tnr = Tenor((*it).first);
			quote = boost::shared_ptr<SimpleQuote>(new SimpleQuote((*it).second));
			
			_quotes[(*it).first ] = quote;
			
			_rateHelpers.push_back(newRateHelper(tnr, quote, type));
		}
		
	}
	
	void RateHelperCurve::build_termstructure(void) 
	{
		double tolerance = 1.0e-15;
        
		if (_todaysDate == Date()) 
			_todaysDate = Settings::instance().evaluationDate();
				
		boost::shared_ptr<YieldTermStructure> ts(
												 new PiecewiseYieldCurve<Discount,LogLinear>(
																							 _fixingDays,
																							 _calendar,
																							 _rateHelpers,
																							 _termStructureDayCounter,
																							 tolerance) 
												 );
		_settlementDate = ts->referenceDate();

		_forecastingTermStructure.linkTo(ts);
		_discountingTermStructure.linkTo(ts);
		
		_yieldTermStructure = ts;
	}
    
    void RateHelperCurve::update(Date todaysDate,
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
        
        for (int i=0; i<depocount; i++) 
            depocurve[depotenors[i]] = depospots[i];
        
        for (int i=0; i<swapcount; i++) 
            swapcurve[swaptenors[i]] = swapspots[i];

        this->update(todaysDate,
                     depocurve,
                     swapcurve,
                     fixingDays);
        
    }
    
    void RateHelperCurve::update(Date todaysDate,
                                 CurveMap depocurve,
                                 CurveMap swapcurve,
                                 int fixingDays) 
    {        
        this->add_depos(depocurve);
        this->add_swaps(swapcurve);
        
        this->build(todaysDate, fixingDays);
        
    }
    
    void RateHelperCurve::update(CurveMap depocurve,
                                 CurveMap swapcurve) 
    {        
        this->add_depos(depocurve);
        this->add_swaps(swapcurve);
        
        this->build();
        
    }

    // Inspectors
    Real RateHelperCurve::tenorquote(string key) 
	{
        return this->_quotes.count(key) > 0 ? this->_quotes[key]->value() : 0.0;
	}
    
}
