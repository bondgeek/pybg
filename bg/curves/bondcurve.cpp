/*
 *  bondcurve.cpp
 *  pybg
 *
 *  Created by BART MOSLEY on 7/29/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#include <bg/curves/bondcurve.hpp>

namespace bondgeek 
{
    BondHelperQuote::BondHelperQuote(const Real   &px_quote,
                                     const Date   &maturity, 
                                     const Rate   &coupon,
                                     Date issue_date):
    _pxquote(px_quote),
    _maturity(maturity),
    _coupon(coupon),
    _issue_date(issue_date)
    {}
    
    boost::shared_ptr<RateHelper> BondCurve::newBondHelper(BondHelperQuote bondquote)
    {
        Date issue_date;
        if (bondquote.issue_date() == Date()) {
            issue_date = _calendar.advance(curveDate(), _fixingDays, Days);
        }
        else {
            issue_date = bondquote.issue_date();
        }

        boost::shared_ptr<Quote> _quote(new SimpleQuote(bondquote.quote()));
        
        Schedule schedule(issue_date, bondquote.maturity(), 
                          Period(_fixedInstrumentFrequency),
                          _calendar, 
                          _fixedInstrumentConvention, _fixedInstrumentConvention,
                          DateGeneration::Backward, false);
        
        boost::shared_ptr<RateHelper> bh(
                                         new FixedRateBondHelper(Handle<Quote>(_quote),
                                                                 _fixingDays,
                                                                 100.0,
                                                                 schedule,
                                                                 std::vector<Rate>(1, bondquote.coupon()),
                                                                 _fixedInstrumentDayCounter,
                                                                 _fixedInstrumentConvention,
                                                                 100.0));
        
        return bh;
        
    }
    
    void BondCurve::add_bonds(BondCurveMap crv) 
    {
        BondCurveMap::iterator it;
        boost::shared_ptr<SimpleQuote> quote;
        Period tnr;
        
        for ( it=crv.begin() ; it != crv.end(); it++ )
        {
            tnr = Tenor((*it).first);
            
            // keep track of quotes so they can be changed in setTenorQuote
            quote = boost::shared_ptr<SimpleQuote>(new SimpleQuote((*it).second.quote()));
            _quotes[ (*it).first ] = quote;
            
            _rateHelpers.push_back(this->newBondHelper((*it).second));
            
        }
    }
    
    void BondCurve::update(BondCurveMap bondcurve,
                           CurveMap     depocurve,
                           Date     todays_date
                           ) 
    {  
        this->setCurveDate(todays_date) ;  
        
        if (this->yieldTermStructurePtr() == NULL) 
        {
            if (!depocurve.empty()) 
                this->add_depos(depocurve);
            if (!bondcurve.empty())
                this->add_bonds(bondcurve);
            
            this->build();
        }
        else 
        {
            if (!depocurve.empty()) 
                this->updateCurve(depocurve);
            if (!bondcurve.empty())
                this->updateBondCurve(bondcurve);
        }
    }
    
    bool BondCurve::updateBondCurve(BondCurveMap crv)
    {
        BondCurveMap::iterator it;
        bool all_good = true;
        
        for ( it=crv.begin() ; it != crv.end(); it++ )
            if ( !setTenorQuote((*it).first, (*it).second.quote()) ) 
            {
                // You can't add tenors via this function
                all_good = false;
            }
        
        // TODO:  We still don't know if we changed EVERY tenor
        return all_good;
    }
    
    
    
}
