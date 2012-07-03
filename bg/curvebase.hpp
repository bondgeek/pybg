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
    protected:
		Date        _todaysDate;
		Date        _settlementDate;
        Calendar    _calendar;
        Integer     _fixingDays;
        
        // Rate Helper parameters
        DayCounter  _depositDayCounter;
        
        // swap parameters
        // TODO:  should be in sub-class?
        Frequency                       _swFixedLegFrequency;
        BusinessDayConvention           _swFixedLegConvention;
        DayCounter                      _swFixedLegDayCounter;
        boost::shared_ptr<IborIndex>    _swFloatingLegIndex;
        
        // Term Structure 
        DayCounter							  _termStructureDayCounter;
		Frequency							  _fixedRateFrequency;
		boost::shared_ptr<YieldTermStructure> _yieldTermStructure;
		
		// Term structures that will be used for pricing:
		RelinkableHandle<YieldTermStructure> _discountingTermStructure;
		RelinkableHandle<YieldTermStructure> _forecastingTermStructure;
       
        void update_parms(CurveBase& crvtype) {
            _calendar = crvtype._calendar;
            _fixingDays = crvtype._fixingDays;
            _depositDayCounter = crvtype._depositDayCounter;
            
            // swap parameters
            _swFixedLegFrequency     = crvtype._swFixedLegFrequency;
            _swFixedLegConvention    = crvtype._swFixedLegConvention;
            _swFixedLegDayCounter    = crvtype._swFixedLegDayCounter;

			_swFloatingLegIndex      = crvtype._swFloatingLegIndex;
            
			// termstructure parameters
            _termStructureDayCounter = crvtype._termStructureDayCounter;
			_fixedRateFrequency      = crvtype._fixedRateFrequency;
            
        }
        
    public:
        CurveBase() {}
        CurveBase(boost::shared_ptr<IborIndex> swFloatingLegIndex,
                  Calendar calendar=TARGET(),
                  Integer fixingDays=2,                  
                  DayCounter depositDayCounter=Actual360(),
                  Frequency fixedRateFrequency=Annual,
                  BusinessDayConvention swFixedLegConvention=Unadjusted, 
                  DayCounter swFixedLegDayCounter=Thirty360(Thirty360::European), 
                  DayCounter termStructureDayCounter=ActualActual(ActualActual::ISDA)
				  ) :
        _calendar(calendar),
        _fixingDays(fixingDays),
        _depositDayCounter(depositDayCounter),
        _fixedRateFrequency(fixedRateFrequency),
        _swFixedLegFrequency(fixedRateFrequency),
        _swFixedLegConvention(swFixedLegConvention),
        _swFixedLegDayCounter(swFixedLegDayCounter),
        _termStructureDayCounter(termStructureDayCounter),
        _swFloatingLegIndex(swFloatingLegIndex) 
		{}
        
        // inspectors
        Calendar						calendar() { return _calendar; }
        Date                            curvedate(void) {return _todaysDate;}
        Integer							fixingDays() { return _fixingDays;}
        DayCounter						depositDayCounter() { return _depositDayCounter; }
        Frequency						fixedLegFrequency() { return _swFixedLegFrequency; }
        BusinessDayConvention			fixedLegConvention() { return _swFixedLegConvention; }
        DayCounter						fixedLegDayCounter() { return _swFixedLegDayCounter; }
        DayCounter						termStructureDayCounter() { return _termStructureDayCounter; }
		Date                            referenceDate() { return this->discountingTermStructure().currentLink()->referenceDate(); }
        Date                            maxDate()  { return this->discountingTermStructure().currentLink()->maxDate(); }
        
        boost::shared_ptr<IborIndex>	floatingLegIndex() { return _swFloatingLegIndex; }
        
		// Each curve type will define its own buid_termstructure, which should assume that
		// the global date is set via Settings.
		// Setting the date is handled in the base class
		void build(const Date &todaysDate, const int &fixingDays=-1);
		void build(void) { build_termstructure(); }
        
		virtual void build_termstructure(void) {}    
		
        // Inspectors for Term Structures
		boost::shared_ptr<YieldTermStructure>	yieldTermStructurePtr() const { return _yieldTermStructure;}
		RelinkableHandle<YieldTermStructure>	discountingTermStructure() const { return _discountingTermStructure; }
		RelinkableHandle<YieldTermStructure>	forecastingTermStructure() const { return _forecastingTermStructure; }
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