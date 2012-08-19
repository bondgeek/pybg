/*
 *  bondcurve.hpp
 *  pybg
 *
 *  Created by BART MOSLEY on 7/29/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#ifndef BONDCURVE_H
#define BONDCURVE_H

#include <bg/curvebase.hpp>
#include <bg/curves/ratehelpercurve.hpp>
#include <bg/date_utilities.hpp>

namespace bondgeek {
    
    class BondHelperQuote 
    {
        Real    _pxquote;
        Date    _maturity; 
        Rate    _coupon;
        Date    _issue_date;
        
    public:
        BondHelperQuote() {}
        BondHelperQuote(const Real   &px_quote,
                        const Date   &maturity, 
                        const Rate   &coupon,
                        Date issue_date=Date()
                        );
        //Inspectors
        Date issue_date() {return _issue_date;}
        Date maturity() {return _maturity;}
        Real coupon() {return _coupon;}
        Real quote() {return _pxquote;}
    };
    
    typedef map<string, BondHelperQuote> BondCurveMap; 
    
    class BondCurve : public RateHelperCurve 
    {
    protected:
        boost::shared_ptr<RateHelper> newBondHelper(BondHelperQuote bondquote);
        
    public:
        BondCurve() {}
        BondCurve(CurveBase crvtype) { update_parms(crvtype); }
        
        void add_bonds(BondCurveMap crv); 
        
        void update(BondCurveMap bondcurve,
                    CurveMap     depocurve=CurveMap(),
                    Date         todays_date=Date()
                    );
        
        bool updateBondCurve(BondCurveMap crv);
    };
}
#endif
