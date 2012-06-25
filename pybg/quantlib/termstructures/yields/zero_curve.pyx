include '../../types.pxi'

from cython.operator cimport dereference as deref
from libcpp.vector cimport vector

cimport _zero_curve as _zc
from pybg.quantlib.handle cimport shared_ptr
from flat_forward cimport YieldTermStructure
from pybg.quantlib.time.daycounter cimport DayCounter
from pybg.quantlib.time.date cimport Date

cdef class ZeroCurve(YieldTermStructure):

    def __init__(self, dates, yields, DayCounter daycounter):

        # convert dates and yields to vector
        cdef vector[_zc.Date]* _date_vector = new vector[_zc.Date]()
        cdef vector[Rate]* _yield_vector = new vector[Rate]()

        # highly inefficient and could be improved
        for date in dates:
            _date_vector.push_back(deref((<Date>date)._thisptr.get()))

        for rate in yields:
            _yield_vector.push_back(rate)

        # create the curve
        self._thisptr = new shared_ptr[_zc.YieldTermStructure](
            new _zc.ZeroCurve(
                deref(_date_vector),
                deref(_yield_vector),
                deref(daycounter._thisptr)
            )
        )
