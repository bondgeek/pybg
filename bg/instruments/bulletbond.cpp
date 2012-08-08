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
                           BusinessDayConvention paymentConvention
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
             paymentConvention),
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
    {
        cout << endl << "bulletbond constr: " << Settings::instance().evaluationDate() << endl;
        cout << "settings: " << &Settings::instance() << endl;
    }
    
    void BulletBond::setEngine(boost::shared_ptr<CurveBase> crvptr)
    {
        boost::shared_ptr<PricingEngine> discEngine = createPriceEngine<DiscountingBondEngine>(
                                                                                               crvptr->discountingTermStructure()
                                                                                               );
        
        Date todaysDate = Settings::instance().evaluationDate();
        cout << "\n\nIn setEnginge(cpp): " << todaysDate << endl;
        cout << "Termstructure Dates: " << crvptr->curveDate() << " | " << crvptr->referenceDate() << endl;
        setPricingEngine(discEngine);
    }

    void BulletBond::setEngine(CurveBase &crv)
    {
        boost::shared_ptr<PricingEngine> discEngine = createPriceEngine<DiscountingBondEngine>(
                                                                                               crv.discountingTermStructure()
                                                                                               );
        
        Date todaysDate = Settings::instance().evaluationDate();
        cout << "\n\nIn setEnginge(cpp): " << todaysDate << endl;
        cout << "Termstructure Dates: " << crv.curveDate() << " | " << crv.referenceDate() << endl;
        setPricingEngine(discEngine);
    }
    
    void BulletBond::setEngine(CurveBase &crv, 
                               Real &a, 
                               Real &sigma,
                               bool lognormal) 
    {
        setEngine(crv);
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