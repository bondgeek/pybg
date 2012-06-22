# distutils: language = c++
# distutils: libraries = QuantLib

include 'quantlib/types.pxi'

from pybg.quantlib.handle cimport shared_ptr

cimport _curves

cdef class RateHelperCurve:
    cdef shared_ptr[_curves.RateHelperCurve]* _thisptr
    
