include '../types.pxi'
from cython.operator cimport dereference as deref

from pybg.quantlib.handle cimport shared_ptr
from pybg.quantlib.time.date cimport Period
from pybg.quantlib.time.daycounter cimport DayCounter
from pybg.quantlib.currency cimport Currency
from pybg.quantlib.time.calendar cimport Calendar

cimport pybg.quantlib._index as _in
cimport pybg.quantlib.indexes._interest_rate_index as _iri

cdef extern from "string" namespace "std":
    cdef cppclass string:
        char* c_str()

from pybg.quantlib.indexes.interest_rate_index cimport InterestRateIndex

cdef class IborIndex(InterestRateIndex):
    def __cinit__(self):
        pass

cdef class OvernightIndex(IborIndex):
    def __cinit__(self):
        pass

