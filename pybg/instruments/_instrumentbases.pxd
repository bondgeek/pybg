# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

# TODO:  not sure we even need to define these classes.

include '../quantlib/types.pxi'
cimport pybg.version

from libcpp cimport bool
from libcpp.vector cimport vector

from pybg.quantlib.time._date cimport Date as _QLDate
from pybg.quantlib.time._period cimport Frequency 
from pybg.quantlib.time._calendar cimport Calendar
from pybg.quantlib.time._calendar cimport BusinessDayConvention
from pybg.quantlib.time._daycounter cimport DayCounter as DayCounter
from pybg.quantlib._cashflow cimport Leg

cimport pybg._curves as _curves

cdef extern from 'bg/instruments/instrumentbase.hpp' namespace 'bondgeek':

    cdef cppclass InstrumentBase:
        InstrumentBase()
        _QLDate get_eval_date()
        void    set_eval_date(_QLDate &date_input) 
        
        # Engines
        void setEngine(_curves.CurveBase crv, 
                       Real              a, 
                       Real              sigma,
                       bool              lognormal
                       ) 


cdef extern from 'bg/instruments/bondbase.hpp' namespace 'bondgeek':

    cdef cppclass BondBase(InstrumentBase):
        BondBase()
        BondBase(Calendar calendar,
                 Natural settlementDays,
                 DayCounter daycounter,
                 Frequency frequency,
                 Real redemption,
                 Real faceamount,
                 BusinessDayConvention accrualConvention,
                 BusinessDayConvention paymentConvention
                 )
        
        # Inspectors
        DayCounter  get_dayCounter()
        Frequency   get_frequency()
        Natural     get_settlementDays() 
        Calendar    get_calendar() 
        Real        get_redemption()
        Real        get_faceAmount() 
        BusinessDayConvention get_accrualConvention() 
        BusinessDayConvention get_paymentConvention() 
        
        #Results
        double toPrice() except +
        double toPrice(Real bondyield) except +
        
        double toYield() except +
        double toYield(Rate bondprice) except +
        
        Real accrued() except +
        

cdef extern from 'ql/instruments/bond.hpp' namespace 'QuantLib':
    cdef cppclass Bond:

        bool isExpired()
        Natural settlementDays()
        Calendar& calendar()
        vector[Real]& notionals()
        Leg cashflows()
        _QLDate maturityDate()
        _QLDate issueDate()
        _QLDate settlementDate()
        bool isTradable(_QLDate d)
        Real accruedAmount() except +
        Real accruedAmount(_QLDate d) except +
