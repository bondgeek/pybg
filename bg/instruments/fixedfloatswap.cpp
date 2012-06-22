/*
 *  fxflswap.cpp
 *  bondgeek
 *
 *  Created by Bart Mosley on 5/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include <bg/instruments/fixedfloatswap.hpp>

namespace bondgeek {
	
	FixedFloatSwap::FixedFloatSwap(const Date &settle,  
								   const Date &maturity, 
								   const Rate &fixedRate, 
								   const boost::shared_ptr< IborIndex > &iborIndex,
								   const Type &payerType,
								   const Spread &floating_spread,
								   const Real &notional,
								   // Fixed Leg
								   const Frequency fixedLegFrequency,
								   const DayCounter fixedLegDayCounter,
								   const BusinessDayConvention fixedLegConvention,
								   // floating leg 
								   const Frequency floatingLegFrequency,
								   const DayCounter floatingLegDayCounter,
								   const BusinessDayConvention floatingLegConvention,
								   const Calendar calendar
								   ) : 
	_settlementDate(settle),
	_maturity(maturity),
	_payerType(payerType),  
	_fixedRate(fixedRate),
	_spread(floating_spread),
	_notional(notional),
	_fixedLegFrequency(fixedLegFrequency),
	_fixedLegDayCounter(fixedLegDayCounter),
	_fixedLegConvention(fixedLegConvention),
	_floatingLegFrequency(floatingLegFrequency),
	_floatingLegDayCounter(floatingLegDayCounter),
	_floatingLegConvention(floatingLegConvention),
	_calendar(calendar),
	VanillaSwap(payerType, 
				notional,
				Schedule(settle, 
						 maturity,
						 Period(fixedLegFrequency),
						 calendar, 
						 fixedLegConvention,
						 fixedLegConvention,
						 DateGeneration::Forward, 
						 false), 
				fixedRate, 
				fixedLegDayCounter,
				Schedule(settle, 
						 maturity,
						 Period(floatingLegFrequency),
						 calendar, 
						 floatingLegConvention,
						 floatingLegConvention,
						 DateGeneration::Forward, 
						 false), 
				iborIndex, 
				floating_spread,
				floatingLegDayCounter) 
    {}
	
	void FixedFloatSwap::setEngine(CurveBase &crv)  
	{
		boost::shared_ptr<PricingEngine> swapEngine = createPriceEngine<DiscountingSwapEngine>(
																							   crv.discountingTermStructure()
																							   );
		
		setPricingEngine(swapEngine);
	}
	
	void FixedFloatSwap::setEngine(CurveBase &crv, 
								   Real &a, 
								   Real &sigma,
								   bool lognormal)  
	{
		setEngine(crv);
	}
	
}