/*
 *  fxflswap.h
 *  bondgeek
 *
 *  Created by Bart Mosley on 5/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef FIXEDFLOATSWAP_H
#define FIXEDFLOATSWAP_H

#include <ql/quantlib.hpp>
#include <bg/indexbase.hpp>
#include <bg/instruments/bginstrument.hpp>

namespace bondgeek {

	class FixedFloatSwap : public BGInstrument, public VanillaSwap {
	protected:
		VanillaSwap::Type       _payerType;
		Rate                    _fixedRate; // = 0.04;
		Spread                  _spread;
		Date                    _settlementDate; 
		Date                    _maturity;
		Real                    _notional; // = 1000000.0;
        
		// parameters
		Calendar _calendar;
		boost::shared_ptr<IborIndex> _iborIndex;
		
		// fixed leg
		Frequency               _fixedLegFrequency; 
		DayCounter              _fixedLegDayCounter; 
		BusinessDayConvention   _fixedLegConvention; 
		
		// floating leg
        Frequency                       _floatingLegFrequency; 
		DayCounter                      _floatingLegDayCounter; 
		BusinessDayConvention           _floatingLegConvention; 
		
		RelinkableHandle<YieldTermStructure> _discountingTermStructure;        
	public:
		FixedFloatSwap(const Date &settle, 
			  const Date &maturity, 
			  const Rate &fixedRate, 
			  const boost::shared_ptr< IborIndex > &iborIndex,
			  const Type &payerType=VanillaSwap::Payer,
			  const Spread &floating_spread=0.0,
			  const Real &notional=1000000.0,
			  // Fixed Leg
			  const Frequency fixedLegFrequency = Semiannual,
			  const DayCounter fixedLegDayCounter = Thirty360(Thirty360::European),
			  const BusinessDayConvention fixedLegConvention = ModifiedFollowing,
			  // floating leg 
			  const Frequency floatingLegFrequency = Quarterly,
			  const DayCounter floatingLegDayCounter = Actual360(),
			  const BusinessDayConvention floatingLegConvention = ModifiedFollowing,
			  const Calendar calendar = TARGET()
			  );
		
		virtual void setEngine(CurveBase &crv) ;
		virtual void setEngine(CurveBase &crv, 
							   Real &a, 
							   Real &sigma,
							   bool lognormal=true) ;		
		
	};

}
#endif
