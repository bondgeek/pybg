# distutils: language = c++
# distutils: libraries = QuantLib

include 'quantlib/types.pxi'

from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.map cimport map
from libcpp.string cimport string

from pybg.quantlib.handle cimport shared_ptr
from pybg.quantlib.time._date cimport Date as _QLDate
from pybg.quantlib.time._period cimport Frequency 
from pybg.quantlib.time._calendar cimport Calendar
from pybg.quantlib.time._calendar cimport BusinessDayConvention
from pybg.quantlib.time._daycounter cimport DayCounter as DayCounter

from pybg.quantlib.indexes._ibor_index cimport IborIndex

from cython.operator cimport dereference as deref


cdef extern from 'bg/instruments/fixedfloatswap.hpp' namespace 'bondgeek':

    cdef enum SwapPayType: 
        FixedReceiver   = -1 
        FixedPayer      = 1 
        
    cdef cppclass FixedFloatSwap:
        FixedFloatSwap(
            _QLDate settle, 
            _QLDate maturity, 
            Rate fixedRate, 
            shared_ptr[IborIndex] iborIndex,
            SwapPayType payerType,
            Spread floating_spread,
            Real notional,
            # Fixed Leg
            Frequency fixedLegFrequency,
            DayCounter fixedLegDayCounter,
            BusinessDayConvention fixedLegConvention,
            # floating leg 
            Frequency floatingLegFrequency,
            DayCounter floatingLegDayCounter,
            BusinessDayConvention floatingLegConvention,
            Calendar calendar
            ) except +  
            