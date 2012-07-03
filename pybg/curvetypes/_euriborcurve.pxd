# distutils: language = c++
# distutils: libraries = QuantLib

from libcpp.string cimport string
from cython.operator cimport dereference as deref

from pybg.quantlib.time._period cimport Frequency as _Frequency

from pybg._curves cimport CurveBase as _CurveBase

cdef extern from 'bg/curves/euriborcurve.hpp' namespace 'bondgeek':
        
    cdef cppclass EURiborCurve(_CurveBase):
        EURiborCurve() except +
        EURiborCurve(string tenor, _Frequency frequency) except +
        
