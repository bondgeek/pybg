# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

from pybg.quantlib.handle cimport shared_ptr

cimport pybg._curves as _curves

# Base curve abstract class
# Specific curve types, e.g. 'USDLiborCurve' should be sub-classed off this
cdef class CurveBase:
    cdef shared_ptr[_curves.CurveBase]* _thisptr
 
cdef class RateHelperCurve:
    cdef shared_ptr[_curves.RateHelperCurve]* _thisptr

cdef class BondHelperQuote:
    cdef shared_ptr[_curves.BondHelperQuote]* _thisptr

cdef class BondCurve(RateHelperCurve):
    pass
