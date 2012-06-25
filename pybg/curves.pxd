# distutils: language = c++
# distutils: libraries = QuantLib

from pybg.quantlib.handle cimport shared_ptr

cimport _curves

# Base curve abstract class
# Specific curve types, e.g. 'USDLiborCurve' should be sub-classed off this
cdef class CurveBase:
    cdef shared_ptr[_curves.CurveBase]* _thisptr
 
cdef class RateHelperCurve:
    cdef shared_ptr[_curves.RateHelperCurve]* _thisptr

