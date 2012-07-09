/*
 *  bulletbond.hpp
 *  bondgeek
 *
 *  Created by BART MOSLEY on 5/19/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */
#ifndef BULLETBOND_H
#define BULLETBOND_H

#include <ql/quantlib.hpp>
#include <bg/indexbase.hpp>
#include <bg/instruments/bginstrument.hpp>

using namespace QuantLib;

namespace bondgeek {
	
    class BulletBond : public BGInstrument, public FixedRateBond {
    protected:
		// Bond type definitions
		Calendar                _calendar;
		Natural                 _settlementDays;
		Frequency               _frequency; 
		DayCounter              _daycounter; 
        Real                    _redemption;
        Real                    _faceamount;
		BusinessDayConvention   _accrualConvention;
		BusinessDayConvention   _paymentConvention; 
		
        Rate _coupon;
		Date _maturity;        
		Date _issue_date;
        
    public:
        BulletBond(const Rate &coupon,
                   const Date &maturity,
                   const Date &issue_date = Date(),
				   Calendar calendar = UnitedStates(UnitedStates::GovernmentBond),
				   Natural settlementDays = 3,
				   DayCounter daycounter = ActualActual(ActualActual::Bond),
				   Frequency frequency = Semiannual,
				   Real redemption = 100.0,
				   Real faceamount = 100.0,
				   BusinessDayConvention accrualConvention = Unadjusted,
				   BusinessDayConvention paymentConvention = Unadjusted
                   );
        
        double toPrice();
        double toPrice(Rate bondyield);
        
        double toYield();
        double toYield(Real bondprice);
		
		virtual void setEngine(CurveBase &crv) ;
		virtual void setEngine(CurveBase &crv, 
							   Real &a, 
							   Real &sigma,
							   bool lognormal=true) ;
		
    };
		
}
#endif