/*
 *  RateHelperCurve.hpp
 *  QuantLibFun
 *
 *  Created by Bart Mosley on 5/18/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef RateHelperCurve_H
#define RateHelperCurve_H

#include <bg/curvebase.hpp>
#include <bg/date_utilities.hpp>

namespace bondgeek {
	
	class RateHelperCurve : public CurveBase {
	protected:
		RHelperList _rateHelpers;
		
		map< string, boost::shared_ptr<SimpleQuote> > _quotes;
		
	public:
		RateHelperCurve() {}
		RateHelperCurve(CurveBase crvtype) { update_parms(crvtype); }
		
		boost::shared_ptr<RateHelper> newRateHelper(const Period &tnr, 
															   const Rate &quote, 
															   const RHType &rhtype);
		
		boost::shared_ptr<RateHelper> newRateHelper(const Period &tnr, 
															   const boost::shared_ptr<Quote> &_quote, 
															   const RHType &rhtype);
		
		void add_ratehelpers(CurveMap, RHType); 
		
		void add_depos(CurveMap crv) { add_ratehelpers(crv, DEPO); }
		void add_swaps(CurveMap crv) { add_ratehelpers(crv, SWAP); }
		
		virtual void build_termstructure(void);
		
		const Real &tenorquote(string key);
		
		Date settlementDate() { return _settlementDate; }
		
		boost::shared_ptr<IborIndex> euribor6MIndex(void);
		
	};

}

#endif