# distutils: language = c++
# distutils: libraries = QuantLib

include 'quantlib/types.pxi'

from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.map cimport map
from libcpp.string cimport string

from pybg.quantlib.handle cimport shared_ptr
from pybg.quantlib.time._date cimport Date as _QLDate

from cython.operator cimport dereference as deref


cdef extern from 'bg/curvebase.hpp' namespace 'bondgeek':
    cdef enum RHType: 
        DEPO
        FRA
        FUT
        SWAP


cdef extern from 'bg/curvebase.hpp' namespace 'bondgeek':
        
    cdef cppclass CurveBase:
        CurveBase() except +
        

cdef extern from 'bg/curves/usdliborcurve.hpp' namespace 'bondgeek':
        
    cdef cppclass USDLiborCurve(CurveBase):
        USDLiborCurve() except +
        USDLiborCurve(string tenor) except +
        

cdef extern from 'bg/curves/ratehelpercurve.hpp' namespace 'bondgeek':
    
    ctypedef map[string, Rate] CurveMap
    
    cdef cppclass RateHelperCurve:
        RateHelperCurve(CurveBase crvtype) except +
        update(_QLDate todaysDate,
               CurveMap &depocurve,
               CurveMap &swapcurve,
               int fixingDays
               )