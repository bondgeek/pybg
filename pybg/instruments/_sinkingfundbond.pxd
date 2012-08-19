# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

include '../quantlib/types.pxi'
cimport pybg.version

from libcpp.vector cimport vector
from pybg.quantlib.handle cimport shared_ptr

from pybg.quantlib.time._date cimport Date as _QLDate
from pybg.quantlib.time._period cimport Frequency 
from pybg.quantlib.time._calendar cimport Calendar
from pybg.quantlib.time._calendar cimport BusinessDayConvention
from pybg.quantlib.time._daycounter cimport DayCounter as DayCounter

from pybg.quantlib._cashflow cimport Leg

cimport pybg._curves as _curves

cdef extern from 'bg/instruments/sinkingfundbond.hpp' namespace 'bondgeek':

    cdef cppclass SinkingFundBond:
        SinkingFundBond(
                   Rate coupon,
                   _QLDate maturity,
                   vector[Real] notionalAmortization,
                   Frequency sinkingfundFrequency,
                   _QLDate issue_date, # = Date()
                   Calendar calendar, # = UnitedStates(UnitedStates::GovernmentBond)
                   Natural settlementDays, # = 3
                   DayCounter daycounter, # = ActualActual(ActualActual::Bond)
                   Frequency frequency, # = Semiannual
                   Real redemption, # = 100.0
                   Real faceamount, # = 100.0
                   BusinessDayConvention accrualConvention, # = Unadjusted
                   BusinessDayConvention paymentConvention, # = Unadjusted
                   _QLDate eval_date #=Date()
                   )
        
        void setEngine(_curves.CurveBase crv)
        void setEngine(shared_ptr[_curves.CurveBase] crvptr)
        
        double toPrice()
        double toPrice(Real bondyield)
        
        double toYield()
        double toYield(Rate bondprice)
        
        # Inspectors
        _QLDate maturityDate()
        _QLDate settlementDate()
        _QLDate settlementDate(_QLDate)
        _QLDate issueDate()
        DayCounter dayCounter()
        Frequency frequency()
        Natural settlementDays()
        Calendar calendar()
        Leg redemptions()
        Leg cashflows()
        _QLDate get_eval_date()
        void set_eval_date(_QLDate)
        