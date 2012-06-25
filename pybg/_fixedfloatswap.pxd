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

from cython.operator cimport dereference as deref


cdef extern from 'bg/instruments/fixedfloatswap.hpp' namespace 'bondgeek':
        
    cdef cppclass FixedFloatSwap:
        FixedFloatSwap(
              const Date &settle, 
			  const Date &maturity, 
			  const Rate &fixedRate, 
			  const boost::shared_ptr< IborIndex > &iborIndex,
			  const Type &payerType=VanillaSwap::Payer,
			  const Spread &floating_spread=0.0,
			  const Real &notional=1000000.0,
			  # Fixed Leg
			  const Frequency fixedLegFrequency = Semiannual,
			  const DayCounter fixedLegDayCounter = Thirty360(Thirty360::European),
			  const BusinessDayConvention fixedLegConvention = ModifiedFollowing,
			  # floating leg 
			  const Frequency floatingLegFrequency = Quarterly,
			  const DayCounter floatingLegDayCounter = Actual360(),
			  const BusinessDayConvention floatingLegConvention = ModifiedFollowing,
			  const Calendar calendar = TARGET()) except +  