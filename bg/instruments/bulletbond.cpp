/*
 *  bulletbond.cpp
 *  bondgeek
 *
 *  Created by Bart Mosley on 6/1/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */


#include <bg/instruments/bulletbond.hpp>

namespace bondgeek {
	BulletBond::BulletBond(const Rate &coupon,
						   const Date &maturity,
						   const Date &issue_date,
						   Calendar calendar,
						   Natural settlementDays,
						   DayCounter daycounter,
						   Frequency frequency,
						   Real redemption,
						   Real faceamount,
						   BusinessDayConvention accrualConvention,
						   BusinessDayConvention paymentConvention
						   ):
	_coupon(coupon),
	_maturity(maturity),
	_issue_date(issue_date),
	_calendar(calendar),
	_settlementDays(settlementDays),
	_daycounter(daycounter),
	_frequency(frequency),
	_redemption(redemption),
	_faceamount(faceamount),
	_accrualConvention(accrualConvention),
	_paymentConvention(paymentConvention),
	FixedRateBond(settlementDays,
				  faceamount,
				  Schedule(issue_date,
						   maturity, 
						   Period(frequency),
						   calendar,
						   accrualConvention, 
						   accrualConvention, 
						   DateGeneration::Backward, 
						   false),
				  std::vector<Rate>(1, coupon),
				  daycounter,
				  paymentConvention,
				  redemption, 
				  issue_date)
	{}
	
	void BulletBond::setEngine(CurveBase &crv)
	{
		boost::shared_ptr<PricingEngine> discEngine = createPriceEngine<DiscountingBondEngine>(
																							   crv.discountingTermStructure()
																							   );
		
		setPricingEngine(discEngine);
	}
	
	void BulletBond::setEngine(CurveBase &crv, 
							   Real &a, 
							   Real &sigma,
							   bool lognormal) 
	{
		setEngine(crv);
	}
	
}