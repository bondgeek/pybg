# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

from libcpp.string cimport string
from cython.operator cimport dereference as deref

from pybg.quantlib.time._period cimport Frequency as _Frequency

from pybg._curves cimport CurveBase as _CurveBase

cdef extern from 'bg/curves/usdliborcurve.hpp' namespace 'bondgeek':
        
    cdef cppclass USDLiborCurve(_CurveBase):
        USDLiborCurve() except +
        USDLiborCurve(string tenor, _Frequency frequency) except +
        
cdef extern from 'bg/curves/euriborcurve.hpp' namespace 'bondgeek':
        
    cdef cppclass EURiborCurve(_CurveBase):
        EURiborCurve() except +
        EURiborCurve(string tenor, _Frequency frequency) except +
        
