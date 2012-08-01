# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

# TODO:  not sure we even need to define these classes.

include '../quantlib/types.pxi'
cimport pybg.version

from libcpp cimport bool

from pybg.quantlib.time._date cimport Date as _QLDate
from pybg.quantlib.time._period cimport Frequency 
from pybg.quantlib.time._calendar cimport Calendar
from pybg.quantlib.time._calendar cimport BusinessDayConvention
from pybg.quantlib.time._daycounter cimport DayCounter as DayCounter

cimport pybg._curves as _curves

cdef extern from 'bg/instruments/instrumentbase.hpp' namespace 'bondgeek':

    cdef cppclass InstrumentBase:
        # Engines
        void setEngine(_curves.CurveBase crv) 
        void setEngine(_curves.CurveBase crv, 
                       Real              a, 
                       Real              sigma,
                       bool              lognormal
                       ) 


cdef extern from 'bg/instruments/bondbase.hpp' namespace 'bondgeek':

    cdef cppclass BondBase(InstrumentBase):
        BondBase(Calendar calendar,
                 Natural settlementDays,
                 DayCounter daycounter,
                 Frequency frequency,
                 Real redemption,
                 Real faceamount,
                 BusinessDayConvention accrualConvention,
                 BusinessDayConvention paymentConvention
                 )
        
        DayCounter get_dayCounter()
        Frequency get_frequency()