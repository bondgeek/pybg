# distutils: language = c++
# distutils: libraries = QuantLib

include 'quantlib/types.pxi'

from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.map cimport map
from libcpp.string cimport string

from pybg.quantlib.handle cimport shared_ptr
from pybg.quantlib.time._date cimport Date as _QLDate
from pybg.quantlib.time._period cimport Frequency 
from pybg.quantlib.time._calendar cimport Calendar

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
        

cdef extern from 'bg/curves/ratehelpercurve.hpp' namespace 'bondgeek':
    
    ctypedef map[string, Rate] CurveMap
    
    cdef cppclass RateHelperCurve(CurveBase):
        RateHelperCurve() except +
        RateHelperCurve(CurveBase crvtype) except +
        
        void add_depos(CurveMap crv)
        void add_swaps(CurveMap crv)
        void add_futs(CurveMap crv)
        
        void build_termstructure()
        
        void update(CurveMap &depocurve,
                    CurveMap &futcurve,
                    CurveMap &swapcurve,
                    _QLDate todaysDate,
                    int fixingDays
                    )
                    
        # Inspectors
        _QLDate     curvedate() 
        _QLDate     referenceDate()
        _QLDate     maxDate()
        Calendar    calendar()
        Integer     fixingDays() 
        
        Real        tenorquote(string key)        
        Real        discount(double years, bool extrapolate)
        Real        discount(_QLDate dfdate, bool extrapolate)

        
