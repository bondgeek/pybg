# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

from libcpp.string cimport string

from cython.operator cimport dereference as deref

from pybg._curves cimport CurveBase as _CurveBase
from pybg.curvetypes._euriborcurve cimport EURiborCurve as _EURiborCurve

from pybg.curves cimport CurveBase

from pybg.quantlib.time._period cimport Frequency as _Frequency
from pybg.quantlib.handle cimport shared_ptr

from pybg.quantlib.time.api import *


cdef class EURiborCurve(CurveBase):
    """Rate Helper Curve
    
    """
            
    def __init__(self, tenor="6M", fxFrequency=Annual):
        
        cdef char* tnr             = tenor
           
        self._thisptr = new shared_ptr[_CurveBase]( \
            new _EURiborCurve(<string>tnr, <_Frequency>fxFrequency)
            )
