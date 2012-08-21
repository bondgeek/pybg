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
                           Frequency payfrequency,
                           Real redemption,
                           Real faceamount,
                           BusinessDayConvention accrualConvention,
                           BusinessDayConvention paymentConvention,
                           Date eval_date
                           ):
    _coupon(coupon),
    _maturity(maturity),
    _issue_date(issue_date),
    BondBase(calendar,
             settlementDays,
             daycounter,
             payfrequency,
             redemption,
             faceamount,
             accrualConvention,
             paymentConvention,
             eval_date),
    FixedRateBond(settlementDays,
                  faceamount,
                  Schedule(issue_date,
                           maturity, 
                           Period(payfrequency),
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
    
    void BulletBond::setEngine(CurveBase &crv, 
                               Real a, 
                               Real sigma,
                               bool lognormal) 
    {
        boost::shared_ptr<PricingEngine> discEngine = \
            createPriceEngine<DiscountingBondEngine>(
                                crv.discountingTermStructure() );

        setPricingEngine(discEngine);
    }
    
    double BulletBond::toPrice(Rate bondyield)
    {
        return this->cleanPrice(bondyield, 
                          this->dayCounter(), 
                          Compounded, 
                          this->frequency());
    }
    
    double BulletBond::toYield(Real bondprice)
    {
        return this->yield(bondprice, 
                          this->dayCounter(), 
                          Compounded, 
                          this->frequency());
    }
    
}