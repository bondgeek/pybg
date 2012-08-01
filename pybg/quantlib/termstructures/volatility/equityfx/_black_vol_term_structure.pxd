include '../../../types.pxi'
cimport pybg.version

from pybg.quantlib.time._calendar cimport Calendar, BusinessDayConvention
from pybg.quantlib.time._date cimport Date
from pybg.quantlib.time._daycounter cimport DayCounter
from pybg.quantlib.time._period cimport Frequency

cdef extern from 'ql/termstructures/voltermstructure.hpp' namespace 'QuantLib':

    cdef cppclass VolatilityTermStructure:
        VolatilityTermStructure()
        VolatilityTermStructure(
            Calendar& cal, 
            BusinessDayConvention bdc,
            DayCounter& dc
        )

cdef extern from 'ql/termstructures/volatility/equityfx/blackvoltermstructure.hpp' namespace 'QuantLib':

    cdef cppclass BlackVolTermStructure(VolatilityTermStructure):
        BlackVolTermStructure()
        BlackVolTermStructure(
            Calendar& cal, 
            BusinessDayConvention bdc,
            DayCounter& dc
        )

cdef extern from 'ql/termstructures/volatility/equityfx/blackconstantvol.hpp' namespace 'QuantLib':

    cdef cppclass BlackConstantVol(BlackVolTermStructure):
        BlackConstantVol()
        BlackConstantVol(Date& referenceDate,
                         Calendar& calendar,
                         Volatility volatility,
                         DayCounter& dayCounter)

