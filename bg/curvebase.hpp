/*
 *  curvetype.hpp
 *  QuantLibFun
 *
 *  Created by BART MOSLEY on 5/16/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#ifndef CURVEBASE_H
#define CURVEBASE_H

#include <ql/quantlib.hpp>

#include <map>
#include <vector>
#include <ostream>

using namespace std;
using namespace QuantLib;

namespace bondgeek {
    // Curves
    typedef map<string, Rate> CurveMap; 
    typedef vector< boost::shared_ptr<RateHelper> > RHelperList;
    
    class CurveBase {
    /* A curve is specified by a set of market instruments.
       Those instruments may include:
        -fixed rate bonds 
        -fixed/floating swaps
        -deposit rates
        -futures on those deposit rates
     
        It is assumed that the market conventions for all fixed rate instruments,
        including swaps, are all consistent for a single curve--as well as for
        all deposit rate instruments.  The conventions may vary between the fixed rate
        instruments and the deposit instruments.
     
     */
    protected:
        Calendar                _calendar;
        Integer                 _fixingDays;
        BusinessDayConvention   _businessDayConvention; 
            
        // fixed rate instrument (bond or swap) parameters
        Frequency                       _fixedInstrumentFrequency;
        BusinessDayConvention           _fixedInstrumentConvention;
        DayCounter                      _fixedInstrumentDayCounter;
        
        
        // Deposit instrument parameters
        DayCounter                      _depositDayCounter;
        boost::shared_ptr<IborIndex>    _swFloatingLegIndex;
        
        // Term Structure 
        DayCounter                              _termStructureDayCounter;
        Frequency                               _fixedRateFrequency;
        boost::shared_ptr<YieldTermStructure>   _yieldTermStructure;
        
        // Term structures that will be used for pricing:
        RelinkableHandle<YieldTermStructure> _discountingTermStructure;
        RelinkableHandle<YieldTermStructure> _forecastingTermStructure;
       
        void update_parms(CurveBase& crvtype) {
            _calendar = crvtype._calendar;
            _fixingDays = crvtype._fixingDays;
            _depositDayCounter = crvtype._depositDayCounter;
            _businessDayConvention = crvtype._businessDayConvention;
            
            // fixed rate instrument (bond or swap) parameters
            _fixedInstrumentFrequency     = crvtype._fixedInstrumentFrequency;
            _fixedInstrumentConvention    = crvtype._fixedInstrumentConvention;
            _fixedInstrumentDayCounter    = crvtype._fixedInstrumentDayCounter;

            _swFloatingLegIndex      = crvtype._swFloatingLegIndex;
            
            // termstructure parameters
            _termStructureDayCounter = crvtype._termStructureDayCounter;
            _fixedRateFrequency      = crvtype._fixedRateFrequency;
            
        }
        
    public:
        CurveBase() {}
        CurveBase(boost::shared_ptr<IborIndex> swFloatingLegIndex,
                  Integer fixingDays=2,
                  Frequency fixedRateFrequency=Annual,
                  BusinessDayConvention fixedInstrumentConvention=Unadjusted, 
                  DayCounter fixedInstrumentDayCounter=Thirty360(Thirty360::European), 
                  DayCounter termStructureDayCounter=ActualActual(ActualActual::ISDA)
                  ) :
        _fixingDays(fixingDays),
        _fixedRateFrequency(fixedRateFrequency),
        _fixedInstrumentFrequency(fixedRateFrequency),
        _fixedInstrumentConvention(fixedInstrumentConvention),
        _fixedInstrumentDayCounter(fixedInstrumentDayCounter),
        _termStructureDayCounter(termStructureDayCounter),
        _swFloatingLegIndex(swFloatingLegIndex) 
        {
            // For a swap curve, the you need to use the floating index
            // calendar to avoid segmentation faults on bad dates.
            // TODO:  this should be fixed in QuantLib's IborIndex Class
            _calendar = _swFloatingLegIndex->fixingCalendar();
            _businessDayConvention = _swFloatingLegIndex->businessDayConvention();
            _depositDayCounter=_swFloatingLegIndex->dayCounter();
        }
        
        CurveBase(Calendar calendar,
                  Integer fixingDays=2,                  
                  DayCounter depositDayCounter=Actual360(),
                  Frequency fixedRateFrequency=Annual,
                  BusinessDayConvention fixedInstrumentConvention=Unadjusted, 
                  DayCounter fixedInstrumentDayCounter=Thirty360(Thirty360::European), 
                  DayCounter termStructureDayCounter=ActualActual(ActualActual::ISDA)
                  ) :
        _calendar(calendar),
        _fixingDays(fixingDays),
        _depositDayCounter(depositDayCounter),
        _fixedRateFrequency(fixedRateFrequency),
        _fixedInstrumentFrequency(fixedRateFrequency),
        _fixedInstrumentConvention(fixedInstrumentConvention),
        _fixedInstrumentDayCounter(fixedInstrumentDayCounter),
        _termStructureDayCounter(termStructureDayCounter)
        {
            _businessDayConvention = _fixedInstrumentConvention;
        }
        
        // Inspectors
        Date                            curveDate(void);
        Calendar                        calendar() { return _calendar; }
        Integer                         fixingDays() { return _fixingDays;}
        DayCounter                      depositDayCounter() { return _depositDayCounter; }
        Frequency                       fixedLegFrequency() { return _fixedInstrumentFrequency; }
        BusinessDayConvention           fixedLegConvention() { return _fixedInstrumentConvention; }
        DayCounter                      fixedLegDayCounter() { return _fixedInstrumentDayCounter; }
        DayCounter                      termStructureDayCounter() { return _termStructureDayCounter; }
        Date                            referenceDate() { return this->discountingTermStructure().currentLink()->referenceDate(); }
        Date                            maxDate()  { return this->discountingTermStructure().currentLink()->maxDate(); }
        
        boost::shared_ptr<IborIndex>    floatingLegIndex() { return _swFloatingLegIndex; }
        
        // Each curve type will define its own build_termstructure, 
        // which should assume that the global date is set via Settings.
        // Setting the date is handled in the base class
        Date setCurveDate(Date todays_date=Date());
        Date advanceCurveDate(int n, TimeUnit unit);
        
        void build(const Date &todays_date);
        void build(void) { 
            build_termstructure(); 
        }
        
        virtual void build_termstructure(void) {}    
        
        // Term Structures
        boost::shared_ptr<YieldTermStructure>   yieldTermStructurePtr() const { return _yieldTermStructure;}
        RelinkableHandle<YieldTermStructure>    discountingTermStructure() const { return _discountingTermStructure; }
        RelinkableHandle<YieldTermStructure>    forecastingTermStructure() const { return _forecastingTermStructure; }
        boost::shared_ptr<YieldTermStructure>   spreadedTermStructurePtr(const Handle<Quote>& spread,
                                                                         Compounding comp = Continuous,
                                                                         Frequency freq = NoFrequency,
                                                                         const DayCounter& dc = DayCounter()
                                                                         ); 
        // values from term structure
        Real discount(double years, bool extrapolate=false); 
        Real discount(const Date &d, bool extrapolate=false);
                
    };
    
    // Price Engines
    template <class engineType>
    boost::shared_ptr<PricingEngine> createPriceEngine(Handle<YieldTermStructure> yieldTermStructure) 
    {
        boost::shared_ptr<PricingEngine> prcEngine(new engineType(yieldTermStructure));
        return prcEngine;
    }
    
    template <class ShortRateModelType, 
              class OptionEngineType>
    boost::shared_ptr<PricingEngine> createOptionEngine(Handle<YieldTermStructure> yieldTermStructure,
                                                  const Real &a, 
                                                  const Real &sigma,
                                                  const Size &timeSteps=50) 
    {
        boost::shared_ptr<ShortRateModel> shortModel(new ShortRateModelType(yieldTermStructure,
                                                                            max(QL_EPSILON, a),
                                                                            max(QL_EPSILON, sigma)
                                                                            )
                                                     );
        boost::shared_ptr<PricingEngine> prxEngine(new OptionEngineType(shortModel,
                                                                        timeSteps)
                                                   );
        
        return prxEngine;
    }
}
#endif