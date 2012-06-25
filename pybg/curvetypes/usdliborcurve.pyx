# distutils: language = c++
# distutils: libraries = QuantLib

from libcpp.string cimport string

from cython.operator cimport dereference as deref

from pybg._curves cimport CurveBase as _CurveBase
from pybg.curvetypes._usdliborcurve cimport USDLiborCurve as _USDLiborCurve

from pybg.curves cimport CurveBase

from pybg.quantlib.time._period cimport Frequency as _Frequency
from pybg.quantlib.handle cimport shared_ptr

from pybg.quantlib.time.api import *


cdef class USDLiborCurve(CurveBase):
    """Rate Helper Curve
    
    """
            
    def __init__(self, tenor="3M", fxFrequency=Semiannual):
        
        cdef char* tnr             = tenor
           
        self._thisptr = new shared_ptr[_CurveBase]( \
            new _USDLiborCurve(<string>tnr, <_Frequency>fxFrequency)
            )
