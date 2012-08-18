/*
 *  SinkingFundBond.h
 *  pybg
 *
 *  Created by Bart Mosley on 8/17/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */
#ifndef SINKINGFUNDBOND_H
#define SINKINGFUNDBOND_H

#include <ql/quantlib.hpp>
#include <bg/indexbase.hpp>
#include <bg/instruments/instrumentbase.hpp>
#include <bg/instruments/bondbase.hpp>

namespace bondgeek
{
    class SinkingFundBond : public BondBase, public Bond 
    {
    public:
        /* Generic Fixed Rate Amortizing Bond 
         
         notionals:  A non-increasing balance of outstanding notionals 
         at each schedule date.
         
         redemptions: A vector of redemption prices for each amortization, assumed to
         be par (100.) if not provided. 
         
         */
        SinkingFundBond(
                        const Rate &coupon,
                        const Date &maturity,
                        const std::vector<Real>& notionalAmortization=std::vector<Real>(),
                        Frequency sinkingfundFrequency=Once,
                        const Date &issue_date = Date(),
                        Calendar paymentCalendar = UnitedStates(UnitedStates::GovernmentBond),
                        Natural settlementDays = 3,
                        DayCounter daycounter = ActualActual(ActualActual::Bond),
                        Frequency payfrequency = Semiannual,
                        Real redemption = 100.0,
                        Real faceamount = 100.0,
                        BusinessDayConvention accrualConvention = Unadjusted,
                        BusinessDayConvention paymentConvention = Unadjusted,
                        Date eval_date=Date()           
                        );
        
        // Inspectors
        Frequency frequency() const { return frequency_; }
        const DayCounter& dayCounter() const { return dayCounter_; }
        
        // Bond Results
        virtual double toPrice(void) { return this->cleanPrice(); }
        virtual double toYield() { return this->toYield(toPrice()); };
        
        virtual double toPrice(Rate bondyield);
        virtual double toYield(Real bondprice);
        
        
        virtual void setEngine(CurveBase &crv) ;
        virtual void setEngine(CurveBase &crv, 
                               Real &a, 
                               Real &sigma,
                               bool lognormal=true) ;
        
        std::vector<Real> sinkingFundNotionals(const std::vector<Real>& sf_par,
                                               Frequency                sf_freq,
                                               const Schedule&          schedule
                                               );
        
    protected:
        Frequency frequency_;
        DayCounter dayCounter_;
        
        void init(
                  Natural                     settlementDays,
                  const Schedule&             schedule,
                  const std::vector<Rate>&    coupons,
                  const DayCounter&           accrualDayCounter,
                  const std::vector<Real>&    notionalAmortization,
                  BusinessDayConvention       paymentConvention = Following,
                  Real                        redemptionAmount = 100.0,
                  const Date&                 issueDate = Date(),
                  const Calendar&             paymentCalendar = Calendar()
                  );
        
        
        
    };
    
}
#endif
