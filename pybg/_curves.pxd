# distutils: language = c++
# distutils: libraries = QuantLib

include 'quantlib/types.pxi'

from libcpp cimport bool
from libcpp.vector cimport vector

from pybg.quantlib.handle cimport shared_ptr
from pybg.quantlib.time._date cimport Date

cdef extern from 'bg/curvebase.hpp' namespace 'bondgeek':
    cdef enum RHType: 
        DEPO
        FRA
        FUT
        SWAP
        
    cdef cppclass CurveBase:
        CurveBase() except +