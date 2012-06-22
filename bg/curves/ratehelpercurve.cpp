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
	boost::shared_ptr<RateHelper> RateHelperCurve::newRateHelper(const Period &tnr, 
														   const Rate &quote, 
														   const RHType &rhtype) 
	{
		boost::shared_ptr<Quote> _quote(new SimpleQuote(quote));
		return newRateHelper(tnr, _quote, rhtype);
	}
	
	boost::shared_ptr<RateHelper> RateHelperCurve::newRateHelper(const Period &tnr, 
														   const boost::shared_ptr<Quote> &_quote, 
														   const RHType &rhtype) 
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
																		Handle<Quote>(Handle<Quote>(_quote)), 
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
		
		/*
		 boost::shared_ptr<YieldTermStructure> ts( new PiecewiseYieldCurve<Discount,LogLinear>(
																	_settlementDate, 
																	_rateHelpers,
																	_termStructureDayCounter,
																	tolerance) 
												 );
		 
		 PiecewiseYieldCurve (Natural settlementDays, 
							  const Calendar &calendar, 
		                      const std::vector< boost::shared_ptr< typename Traits::helper > > &instruments, 
							  const DayCounter &dayCounter, 
		                      Real accuracy, 
							  const Interpolator &i=Interpolator(), 
		                      const Bootstrap< this_curve > &bootstrap=Bootstrap< this_curve >())
		 */
		_forecastingTermStructure.linkTo(ts);
		_discountingTermStructure.linkTo(ts);
		
		_yieldTermStructure = ts;
	}

	boost::shared_ptr<IborIndex> RateHelperCurve::euribor6MIndex() 
	{
		boost::shared_ptr<IborIndex> euriborIndex(
												  new Euribor6M(forecastingTermStructure()));
		return euriborIndex;
	}

	const Real &RateHelperCurve::tenorquote(string key) 
	{
        const Real &q = _quotes.count(key) > 0 ? _quotes[key]->value() : 0.0;
		return q;
	}
	
}
