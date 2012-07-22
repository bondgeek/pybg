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
                return out << "ED Futures";
            case SWAP:
                return out << "Swaps";
            default:
                QL_FAIL("unknown RateHelperType");
        }
    } 
    
    boost::shared_ptr<RateHelper> RateHelperCurve::newRateHelper(const Period &tnr, 
                                                                 const Rate &quote, 
                                                                 const RHType &rhtype,
                                                                 int forwardPeriod
                                                                 ) 
    {
        boost::shared_ptr<Quote> _quote(new SimpleQuote(quote));
        
        return newRateHelper(tnr, _quote, rhtype, forwardPeriod);
    }
    
    boost::shared_ptr<RateHelper> RateHelperCurve::newRateHelper(const Period &tnr, 
                                                                 const boost::shared_ptr<Quote> &_quote, 
                                                                 const RHType &rhtype,
                                                                 int forwardPeriod
                                                                 ) 
    {
        Date forwardDate = Settings::instance().evaluationDate();
        
        switch(rhtype) {
            case DEPO:
                return boost::shared_ptr<RateHelper>(new DepositRateHelper(
                                                                           Handle<Quote>(_quote),
                                                                           tnr, 
                                                                           _fixingDays,
                                                                           _calendar, 
                                                                           ModifiedFollowing,
                                                                           true, 
                                                                           _depositDayCounter)
                                                     );
                break;
            
            case SWAP:
                return boost::shared_ptr<RateHelper>(new SwapRateHelper(
                                                                        Handle<Quote>(_quote), 
                                                                        tnr,
                                                                        _calendar, 
                                                                        _fixedInstrumentFrequency,
                                                                        _fixedInstrumentConvention, 
                                                                        _fixedInstrumentDayCounter,
                                                                        _swFloatingLegIndex)
                                                     );
                break;
            
            case FUT:
                for (int i=0; i<forwardPeriod; i++) 
                    forwardDate = IMM::nextDate(forwardDate);
                
                return boost::shared_ptr<RateHelper>(new FuturesRateHelper(
                                                                        Handle<Quote>(_quote), 
                                                                        forwardDate,
                                                                        tnr.length(),
                                                                        _calendar, 
                                                                        ModifiedFollowing,
                                                                        true,
                                                                        _depositDayCounter)
                                                     );
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
        int fwdPeriod = 0;

        for ( it=crv.begin() ; it != crv.end(); it++ )
        {
            // keep track of quotes so they can be changed in setTenorQuote
            quote = boost::shared_ptr<SimpleQuote>(new SimpleQuote((*it).second));
            _quotes[ (*it).first ] = quote;
            
            switch (type) 
            {
                case FUT:
                    tnr = Tenor("3M");
                    fwdPeriod = FuturesTenor((*it).first);
                    break;
                    
                default:
                    tnr = Tenor((*it).first);
                    break;
            }
            
            _rateHelpers.push_back(newRateHelper(tnr, quote, type, fwdPeriod));
            
            
        }
        
    }
    
    void RateHelperCurve::build_termstructure(void) 
    {
        double tolerance = 1.0e-12;
        
        Settings::instance().evaluationDate();
        
        boost::shared_ptr<YieldTermStructure> ts(
                                                 new PiecewiseYieldCurve<Discount,LogLinear>(_fixingDays,
                                                                                             this->calendar(), 
                                                                                             _rateHelpers,
                                                                                             _termStructureDayCounter,
                                                                                             tolerance) 
                                                 );
        
        _forecastingTermStructure.linkTo(ts);
        _discountingTermStructure.linkTo(ts);
        
        _yieldTermStructure = ts;
    }
    
    void RateHelperCurve::update(std::string depotenors[],
                                 double depospots[],
                                 int depocount,                          
                                 std::string swaptenors[],
                                 double swapspots[],
                                 int swapcount,
                                 Date todaysDate
                                 ) 
    {
        CurveMap depocurve;
        CurveMap swapcurve;
        CurveMap futcurve;
        
        for (int i=0; i<depocount; i++) 
            depocurve[depotenors[i]] = depospots[i];
        
        for (int i=0; i<swapcount; i++) 
            swapcurve[swaptenors[i]] = swapspots[i];

        this->update(depocurve,
                     futcurve,
                     swapcurve,
                     todaysDate);
        
    }
        
    void RateHelperCurve::update(CurveMap depocurve,
                                 CurveMap futcurve,
                                 CurveMap swapcurve,
                                 Date     todays_date) 
    {  
        this->setCurveDate(todays_date) ;  
        
        if (this->yieldTermStructurePtr() == NULL) 
        {
            if (!depocurve.empty()) 
                this->add_depos(depocurve);
            if (!futcurve.empty())
                this->add_futs(futcurve);
            if (!swapcurve.empty())
                this->add_swaps(swapcurve);
            
            this->build();
        }
        else 
        {
            if (!depocurve.empty()) 
                this->updateCurve(depocurve);
            if (!futcurve.empty())
                this->updateCurve(futcurve);
            if (!swapcurve.empty())
                this->updateCurve(swapcurve);
        }

    }

    // Inspectors
    Real RateHelperCurve::tenorquote(string key) 
    {
        return this->_quotes.count(key) > 0 ? this->_quotes[key]->value() : 0.0;
    }
    
    CurveMap RateHelperCurve::curveQuotes()
    {
        string key;
        CurveMap crv;
        map< string, boost::shared_ptr<SimpleQuote> >::iterator it;
        
        for (it=this->_quotes.begin(); it != this->_quotes.end(); it++)
        {
            crv[(*it).first] = (*it).second->value();
        }
        
        return crv;
    }
    
    // Setters
    bool RateHelperCurve::setTenorQuote(string key, Real quoteValue)
    {
        // if key exists, that means there is a ratehelper, so update the quote
        // if not fail, because something else should be done.
        boost::shared_ptr<SimpleQuote> newValue;
        
        if ( this->_quotes.count(key) > 0 )
        {
            newValue =  boost::dynamic_pointer_cast<SimpleQuote>(this->_quotes[key]);
            newValue->setValue(quoteValue);
            return true;
        }
        
        return false;
    }
    
    bool RateHelperCurve::updateCurve(CurveMap crv)
    {
        CurveMap::iterator it;
        bool all_good = true;
        
        for ( it=crv.begin() ; it != crv.end(); it++ )
            if ( !setTenorQuote((*it).first, (*it).second) ) 
            {
                // You can't add tenors via this function
                all_good = false;
            }
        
        // TODO:  We still don't know if we changed EVERY tenor
        return all_good;
    }
}
