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
        
        virtual double toPrice(void) { return this->cleanPrice(); }
        virtual double toYield() { return this->toYield(toPrice()); }
        
        virtual double toPrice(Rate bondyield);
        virtual double toYield(Real bondprice);
        
        // alpha, sigma & lognormal parameters are ignored for bullet/noncall
        virtual void setEngine(CurveBase &crv, 
                               Real a=QL_EPSILON, 
                               Real sigma=QL_EPSILON,
                               bool lognormal=true) ;
        
    };
    
}
#endif