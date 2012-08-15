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
#include <bg/instruments/instrumentbase.hpp>
#include <bg/instruments/bondbase.hpp>

using namespace QuantLib;

namespace bondgeek {
    
    class BulletBond : public BondBase, public FixedRateBond {
    protected:
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
                   Frequency payfrequency = Semiannual,
                   Real redemption = 100.0,
                   Real faceamount = 100.0,
                   BusinessDayConvention accrualConvention = Unadjusted,
                   BusinessDayConvention paymentConvention = Unadjusted,
                   Date eval_date=Date()
                   );
        
        virtual double toPrice(void);
        virtual double toPrice(Rate bondyield);
        virtual double toYield(Real bondprice);
        virtual double toYield() {return toYield(toPrice()); };
        
        virtual void setEngine(CurveBase &crv) ;
        virtual void setEngine(CurveBase &crv, 
                               Real &a, 
                               Real &sigma,
                               bool lognormal=true) ;
        
    };
 
    // Inline functions
    inline double BulletBond::toPrice()
    {
        return this->cleanPrice();
    }
    
}
#endif