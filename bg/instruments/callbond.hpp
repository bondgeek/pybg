/*
 *  callbond.h
 *  bondgeek
 *
 *  Created by BART MOSLEY on 6/16/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */
#ifndef CALLBOND_H
#define CALLBOND_H

#include <ql/quantlib.hpp>
#include <bg/indexbase.hpp>
#include <bg/instruments/bginstrument.hpp>

using namespace QuantLib;

namespace bondgeek {
	
	// CallabilitySchedule generator:
	//
	//   typedef std::vector<boost::shared_ptr<Callability> > CallabilitySchedule;
	// 
	CallabilitySchedule createBondCallSchedule(const Date &firstCall,
											   const Date &endDate, 
											   const Real &callPrice=100.0,
											   const Frequency &callFrequency=Annual
											   );
	
	class CallBond : public BGInstrument, public CallableFixedRateBond 
	{
	protected:
		// Bond type definitions
		Calendar                _calendar;
		Natural                 _settlementDays;
		Frequency               _payfrequency;
		DayCounter              _daycounter; 
        Real                    _redemption;
        Real                    _faceamount;
		BusinessDayConvention   _accrualConvention;
		BusinessDayConvention   _paymentConvention; 
		
        Rate _coupon;
		Date _maturity;
		Date _callDate;
		Real _callPrice;
		Date _issue_date;
        
        // OAS Calcs
		RelinkableHandle<YieldTermStructure> _spreadedTermStructure;
        boost::shared_ptr<SimpleQuote>       _curveSpread; 
        Real                                 _reversionParameter;
        Real                                 _sigma;
        bool                                 _lognormal;
        
		
		class OASSolver
		{
			CallBond *bond;
			Real     objPrice;
			bool     impliedVol;
		public:
			OASSolver(CallBond *bnd, 
					  const Real &objPx, 
					  bool impliedVolCalc=false) : 
			bond(bnd), 
			objPrice(objPx),
			impliedVol(impliedVolCalc) 
			{}
			
			Real operator()(Real x) const {
				if (!impliedVol) {
					return bond->oasValue(x) - objPrice;
					
				} else {
					Real xsprd = bond->_curveSpread->value();
					Real mr = bond->reversionParameter();
					return bond->oasValue(xsprd, x, mr, bond->lognormal()) - objPrice;
					
				}
			}
		};
		
	public:
		CallBond(const Rate &coupon,
				 const Date &maturity,
				 const Date &callDate,
				 const Real &callPrice, 
				 const Date &issue_date = Date(),
				 Calendar calendar = UnitedStates(UnitedStates::GovernmentBond),
				 Natural settlementDays = 3,
				 DayCounter daycounter = ActualActual(ActualActual::Bond),
				 Frequency payFrequency = Semiannual,
				 Frequency callFrequency = Semiannual,
				 Real redemption = 100.0,
				 Real faceamount = 100.0,
				 BusinessDayConvention accrualConvention = Unadjusted,
				 BusinessDayConvention paymentConvention = Unadjusted
				 );
		CallBond(const Rate &coupon,
				 const Date &maturity,
				 const Date &issue_date = Date(),
				 Calendar calendar = UnitedStates(UnitedStates::GovernmentBond),
				 Natural settlementDays = 3,
				 DayCounter daycounter = ActualActual(ActualActual::Bond),
				 Frequency payFrequency = Semiannual,
				 Real redemption = 100.0,
				 Real faceamount = 100.0,
				 BusinessDayConvention accrualConvention = Unadjusted,
				 BusinessDayConvention paymentConvention = Unadjusted
				 );
		// Constructor for noncall bond
		
        // Inspectors
        Real reversionParameter() {return _reversionParameter; }
        Real sigma() { return _sigma; }
        bool lognormal() { return _lognormal; } 
        
        // Engines
		virtual void setEngine(CurveBase &crv) ;
		virtual void setEngine(CurveBase &crv, 
							   Real &a, 
							   Real &sigma,
							   bool lognormal=true) ;
        
		
		// OAS Functions
        void    oasEngine(CurveBase &crv, 
                          Real &a, 
                          Real &sigma,
                          const Real &spread=0.0,
                          bool lognormal=true);
        void    oasEngine(Real &a, 
                          Real &sigma,
                          const Real &spread=0.0,
                          bool lognormal=true);
        
		void setSpread(const Real &spread);
        Real getSpread(void);
        
		// OAS Functions
		// Functions to price bond off oas engine, given spread and vol
        Real oasValue(const Real &spread);
        Real oasValue(const Real &spread, Real &sigma, Real a=0.0, bool lognormal=true);
		
		// Solve for spread or implied volatility
        Real oas(const Real &bondprice,
                 Real sigma, 
                 Real a=0.0,
                 bool lognormal=true
				 );
		
		Real oasImpliedVol(const Real &bondprice, 
                           Real spread,
                           Real a=0.0,
                           bool lognormal=true
						   );
		
		// Template Functions
		template <class SolverClass>
		Real oasT(const Real &bondprice,
				  Real sigma, 
				  Real a=0.0,
				  bool lognormal=true,
				  Real step = 0.001,
				  Real accuracy=1.0e-7,
				  Real guess = 0.0
				  )
		{
			SolverClass solver;
			OASSolver oasSolver(this, bondprice);
			
			oasEngine(a, sigma, guess, lognormal);
			
			Real root = solver.solve(oasSolver, accuracy, guess, step);
			
			return root;
		}

		template<class SolverClass>
        Real oasImpliedVolT(const Real &bondprice, 
                           Real spread,
                           Real a=0.0,
							bool lognormal=true,
							Real step = 0.001,
							Real accuracy=1.0e-7,
							Real guess = 0.01
						   )
        {            
            SolverClass solver;
            OASSolver oasSolver(this, bondprice, true);
            
            oasEngine(a, guess, spread, lognormal);
			
            Real root = solver.solve(oasSolver, accuracy, guess, step);
            
            return root;            
            
        }
	};
	
}
#endif