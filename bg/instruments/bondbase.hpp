/*
 *  BondBase.hpp
 *  bondgeek
 *
 *  Created by Bart Mosley on 7/28/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#ifndef BONDBASE_H
#define BONDBASE_H

#include <ql/quantlib.hpp>
#include <bg/instruments/instrumentbase.hpp>
#include <bg/curvebase.hpp>

using namespace QuantLib;

namespace bondgeek 
{
    
    class BondBase : public InstrumentBase {
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
        
    public:
        BondBase() {};
        BondBase(Calendar calendar,
                 Natural settlementDays,
                 DayCounter daycounter,
                 Frequency frequency,
                 Real redemption,
                 Real faceamount,
                 BusinessDayConvention accrualConvention,
                 BusinessDayConvention paymentConvention,
                 Date eval_date=Date()
                 ) :
        _calendar(calendar),
        _settlementDays(settlementDays),
        _daycounter(daycounter),
        _payfrequency(frequency),
        _redemption(redemption),
        _faceamount(faceamount),
        _accrualConvention(accrualConvention),
        _paymentConvention(paymentConvention),
        InstrumentBase(eval_date)
        {}
        
        virtual double toYield() =0;
        virtual double toYield(Real bondprice) = 0;

        virtual double toPrice() =0;
        virtual double toPrice(Rate bondyield) =0;        
        
        virtual double accrued() =0;
        
        // Inspectors not available in all QuantLib classes
        DayCounter  get_dayCounter() { return _daycounter; }
        Frequency   get_frequency() { return _payfrequency; }
        Natural     get_settlementDays() { return _settlementDays; }
        Calendar    get_calendar() { return _calendar; }
        Real        get_redemption() { return _redemption; } 
        Real        get_faceAmount() { return _faceamount; } 
        BusinessDayConvention get_accrualConvention() { return _accrualConvention; }
        BusinessDayConvention get_paymentConvention() { return _paymentConvention; }
                    
    };
    
}
#endif