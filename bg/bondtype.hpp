/*
 *  bondtype.hpp
 *  bondgeek
 *
 *  Created by BART MOSLEY on 5/19/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */
#ifndef BONDTYPE_H
#define BONDTYPE_H

#include <ql/quantlib.hpp>
#include <bg/instruments/bulletbond.hpp>
#include <bg/instruments/callbond.hpp>

using namespace QuantLib;

namespace bondgeek {	
	
	// TODO: Not a Model element, should probably be in python-based Controller 
	class BondType {
    protected:
		Calendar                _calendar;
		Natural                 _settlementDays;
		Frequency               _frequency; 
		DayCounter              _daycounter; 
        Real                    _redemption;
        Real                    _faceamount;
		BusinessDayConvention   _accrualConvention;
		BusinessDayConvention   _paymentConvention; 
		
	public:
        BondType(Calendar calendar = UnitedStates(UnitedStates::GovernmentBond),
				 Natural settlementDays = 3,
				 DayCounter daycounter = ActualActual(ActualActual::Bond),
				 Frequency frequency = Semiannual,
				 Real redemption = 100.0,
				 Real faceamount = 100.0,
				 BusinessDayConvention accrualConvention = Unadjusted,
				 BusinessDayConvention paymentConvention = Unadjusted
				 ) :
        _calendar(calendar),
        _settlementDays(settlementDays),
        _daycounter(daycounter),
        _frequency(frequency),
        _redemption(redemption),
        _faceamount(faceamount),
        _accrualConvention(accrualConvention),
        _paymentConvention(paymentConvention)
        {}
		
		boost::shared_ptr<BulletBond> createBullet(const Rate &coupon, 
												   const Date &maturity, 
												   const Date &issue_date = Date()
												   )
		{
			boost::shared_ptr<BulletBond> bnd(new BulletBond(coupon,
															 maturity,
															 issue_date,
															 _calendar,
															 _settlementDays,
															 _daycounter,
															 _frequency,
															 _redemption,
															 _faceamount,
															 _accrualConvention,
															 _paymentConvention));
			return bnd;
		}
	};

}
#endif
