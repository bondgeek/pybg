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
    
    // Rate Helper Types
    enum RHType {DEPO, FRA, FUT, SWAP};
	
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
		
        void update(Date todaysDate,
                    std::string depotenors[],
                    double depospots[],
                    int depocount,                          
                    std::string swaptenors[],
                    double swapspots[],
                    int swapcount,
                    int fixingDays = -1
                    );        

        void update(Date todaysDate,
                    CurveMap depocurve,
                    CurveMap swapcurve,
                    int fixingDays = -1
                    );
        
        void update(CurveMap depocurve,
                    CurveMap swapcurve
                    );
        
        // Inspectors
        Real tenorquote(string key);

	};

}

#endif