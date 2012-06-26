/*
 *  swaps.hpp	
 *  bondgeek
 *
 *  Created by Bart Mosley on 5/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef SWAPTYPE_H
#define SWAPTYPE_H

#include <ql/quantlib.hpp>
#include <bg/indexbase.hpp>
#include <bg/instruments/fixedfloatswap.hpp>

using namespace std;
using namespace QuantLib;

namespace bondgeek {
  
	
	// TODO: Not a Model element, should probably be in python-based Controller 
	template <class T>
	class SwapType {
    protected:
		Calendar _calendar;
		
		IndexBase<T>  _indexBase;
		
		// fixed leg
		Frequency               _fixedLegFrequency; 
		DayCounter              _fixedLegDayCounter; 
		BusinessDayConvention   _fixedLegConvention; 
		
		// floating leg
        Frequency                       _floatingLegFrequency; 
		DayCounter                      _floatingLegDayCounter; 
		BusinessDayConvention           _floatingLegConvention; 
		
	public:
        SwapType(// Fixed Leg
				  Frequency fixedLegFrequency = Semiannual,
				  DayCounter fixedLegDayCounter = Thirty360(Thirty360::European),
				  BusinessDayConvention fixedLegConvention = ModifiedFollowing,
				  // floating leg 
				  Frequency floatingLegFrequency = Quarterly,
				  DayCounter floatingLegDayCounter = Actual360(),
				  BusinessDayConvention floatingLegConvention = ModifiedFollowing,
				  Calendar calendar = TARGET()
				  ) :
        _fixedLegFrequency(fixedLegFrequency),
        _fixedLegDayCounter(fixedLegDayCounter),
        _fixedLegConvention(fixedLegConvention),
        _floatingLegFrequency(floatingLegFrequency),
        _floatingLegDayCounter(floatingLegDayCounter),
        _floatingLegConvention(floatingLegConvention),
		_calendar(calendar) 
		{
			_indexBase = IndexBase<T>(floatingLegFrequency);
		}
        
		void linkIndexTo(const boost::shared_ptr<YieldTermStructure> &yieldTermStructurePtr) {
			_indexBase.linkTo(yieldTermStructurePtr);
		}
		
		boost::shared_ptr<FixedFloatSwap> create(Date settle, 
												 Date maturity, 
												 Rate fixedRate,
												 SwapPayType payerType = FixedPayer,
												 Spread floating_spread = 0.0,
												 Real notional = 1000000.0
												 )
		{
			boost::shared_ptr<IborIndex> iborIdx = _indexBase();
		
			boost::shared_ptr<FixedFloatSwap> swp(new FixedFloatSwap(settle,
																	 maturity,
																	 fixedRate,
																	 iborIdx,
																	 payerType,
																	 floating_spread,
																	 notional,
																	 _fixedLegFrequency,
																	 _fixedLegDayCounter,
																	 _fixedLegConvention,
																	 _floatingLegFrequency,
																	 _floatingLegDayCounter,
																	 _floatingLegConvention,
																	 _calendar));
			return swp;
		}
		
	};
	
}
#endif
