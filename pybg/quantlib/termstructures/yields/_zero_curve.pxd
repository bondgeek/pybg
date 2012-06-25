# distutils: language = c++
# distutils: libraries = QuantLib

include '../../types.pxi'

from libcpp.vector cimport vector

from _flat_forward cimport YieldTermStructure
from pybg.quantlib.time._date cimport Date
from pybg.quantlib.time._daycounter cimport DayCounter

cdef extern from 'ql/termstructures/yield/zerocurve.hpp' namespace 'QuantLib':

    cdef cppclass ZeroCurve(YieldTermStructure):

        ZeroCurve(
            vector[Date]& dates,
            vector[Rate]& yields,
            DayCounter& dayCounter
        ) except +
