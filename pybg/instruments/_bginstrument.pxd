# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

include '../quantlib/types.pxi'

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

