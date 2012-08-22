/*
 *  SinkingFundBond.cpp
 *  pybg
 *
 *  Created by Bart Mosley on 8/17/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#include <bg/instruments/sinkingfundbond.hpp>


namespace bondgeek
{
    SinkingFundBond::SinkingFundBond(const Rate &coupon,
                                     const Date &maturity,
                                     const std::vector<Real>& sinkingfund,
                                     Frequency sinkingfundFrequency,
                                     const Date &issue_date,
                                     Calendar paymentCalendar,
                                     Natural settlementDays,
                                     DayCounter daycounter,
                                     Frequency payfrequency,
                                     Real redemption,
                                     Real faceamount,
                                     BusinessDayConvention accrualConvention,
                                     BusinessDayConvention paymentConvention,
                                     Date eval_date) : 
    BondBase(paymentCalendar,
             settlementDays,
             daycounter,
             payfrequency,
             redemption,
             faceamount,
             accrualConvention,
             paymentConvention,
             eval_date),
    Bond(settlementDays,
         paymentCalendar,
         issue_date),
    frequency_(payfrequency),
    dayCounter_(daycounter) 
    {
        Schedule schedule(issue_date,
                          maturity, 
                          Period(payfrequency),
                          paymentCalendar,
                          accrualConvention, 
                          accrualConvention, 
                          DateGeneration::Backward, 
                          false);
        
        std::vector<Real> sfnotional;
        if (!sinkingfund.empty()) {
            sfnotional = this->sinkingFundNotionals(sinkingfund,
                                                    sinkingfundFrequency,
                                                    schedule);
            // TODO: add faceamount as argument to sinkingFundNotionals
        } else {
            sfnotional = std::vector<Real>(1, faceamount);
        }
        
        this->init(settlementDays,
                   schedule,
                   std::vector<Rate>(1, coupon),
                   daycounter,
                   sfnotional,
                   paymentConvention,
                   redemption,
                   issue_date,
                   paymentCalendar
                   );        
        
    }
    
    
    void SinkingFundBond::init(
                               Natural                     settlementDays,
                               const Schedule&             schedule,
                               const std::vector<Rate>&    coupons,
                               const DayCounter&           accrualDayCounter,
                               const std::vector<Real>&    notionalAmortization,
                               BusinessDayConvention       paymentConvention,
                               Real                        redemptionAmount,
                               const Date&                 issueDate,
                               const Calendar&             paymentCalendar
                               ) 
    {
        maturityDate_ = schedule.endDate();

        cashflows_ = FixedRateLeg(schedule)
        .withNotionals(notionalAmortization)
        .withCouponRates(coupons, accrualDayCounter)
        //check quantlib version
        //.withPaymentCalendar(calendar_)
        .withPaymentAdjustment(paymentConvention);
        
        addRedemptionsToCashflows(std::vector<Real>(1, redemptionAmount));        
        
    }
    
    std::vector<Real> SinkingFundBond::sinkingFundNotionals(
                                                            const std::vector<Real>& sf_par,
                                                            Frequency                sf_freq,
                                                            const Schedule&          schedule
                                                            )
    {
        Frequency            frequency = schedule.tenor().frequency();
        unsigned int         sched_size = schedule.size();
        
        std::vector<Real> sf_notionals(sched_size, 0.0);
        
        //Total amount sunk
        double sf_total=0.0;
        for (unsigned int i=0; i < sf_par.size(); i++)
            sf_total += sf_par[i];

        int nsub = numSubPeriod(Period(frequency), Period(sf_freq));
        QL_ENSURE(nsub > 0, 
                  "sink frequency not compatible with bond schedule");
        QL_ENSURE(sf_par.size() <= sched_size, 
                  "sink schedule longer than bond schedule");

        // prior to sinking fund start all notionals are par.
        int sf_start_pos = sched_size - (1 + nsub * sf_par.size()); 
        for (int i = 0; i < sf_start_pos; i++)
        {
            sf_notionals[i] = 100.;
        }

        double bal = 100. ;
        int n_itr = 0;
        for (unsigned int i=sf_start_pos; i < sched_size; i=i+nsub) 
        {
            for (int j = 0; j < nsub; j++) 
            {
                if (i+j < sched_size)
                {
                    sf_notionals[i+j] = bal;
                }
            }
            
            if (bal > QL_EPSILON) 
            {
                bal -= 100.0 * sf_par[n_itr] / sf_total;
                n_itr++; 
            }
        }

        return sf_notionals;
    }
     
    void SinkingFundBond::setEngine(CurveBase &crv, 
                                    Real a, 
                                    Real sigma,
                                    bool lognormal) 
    {
        boost::shared_ptr<PricingEngine> discEngine = \
            createPriceEngine<DiscountingBondEngine>(
                                           crv.discountingTermStructure()
                                           );
        
        this->setPricingEngine(discEngine);
    }
    
    double SinkingFundBond::toPrice(Rate bondyield)
    {
        return this->cleanPrice(bondyield, 
                                this->dayCounter(), 
                                Compounded, 
                                this->frequency());
    }
    
    double SinkingFundBond::toYield(Real bondprice)
    {
        return this->yield(bondprice, 
                           this->dayCounter(), 
                           Compounded, 
                           this->frequency());
    }
    
}

