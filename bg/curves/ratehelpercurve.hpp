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
        RateHelperCurve(CurveBase crvtype) { 
            update_parms(crvtype); 
            cout << "\nin constructor RateHelperCurve: " << Settings::instance().evaluationDate() << endl;    
            cout << "settings: " << &Settings::instance() << endl;
        }
        
        boost::shared_ptr<RateHelper> newRateHelper(const Period &tnr, 
                                                    const Rate &quote,
                                                    const RHType &rhtype,
                                                    int forwardPeriod=0);
        
        boost::shared_ptr<RateHelper> newRateHelper(const Period &tnr,
                                                    const boost::shared_ptr<Quote> &_quote, 
                                                    const RHType &rhtype,
                                                    int forwardPeriod=0);
        
        void add_ratehelpers(CurveMap, RHType); 
        
        void add_depos(CurveMap crv) { add_ratehelpers(crv, DEPO); }
        void add_swaps(CurveMap crv) { add_ratehelpers(crv, SWAP); }
        void add_futs(CurveMap crv) { add_ratehelpers(crv, FUT); }
        
        virtual void build_termstructure(void);
        
        void update(std::string depotenors[],
                    double depospots[],
                    int depocount,                          
                    std::string swaptenors[],
                    double swapspots[],
                    int swapcount,
                    Date todays_date=Date()
                    );        

        void update(CurveMap depocurve=CurveMap(),
                    CurveMap futcurve=CurveMap(),
                    CurveMap swapcurve=CurveMap(),
                    Date todays_date=Date()
                    );
        
        // Inspectors
        Real        tenorquote(string key);
        CurveMap    curveQuotes(void);
        
        // Setters
        bool setTenorQuote(string key, Real quoteValue);
        bool updateCurve(CurveMap crv);
        
    };

}

#endif