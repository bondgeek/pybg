# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like
include '../quantlib/types.pxi'

cimport pybg.version
cimport pybg.instruments._instrumentbases as _instrumentbases

from libcpp cimport bool

from pybg.quantlib.handle cimport shared_ptr

cdef class BondBase:
    cdef shared_ptr[_instrumentbases.BondBase]* _thisptr
    
    cdef object _evaldate
    cdef bool _callable
    cdef Rate _coupon
    cdef object _maturity
    cdef object _firstCallDate
    cdef Real _firstCallPrice
    cdef object _parCallDate
    cdef object _issueDate
    cdef object _sinkingfund
    cdef object _sinkingfundFrequency
    cdef object _callfrequency
    cdef bool _sinker
    