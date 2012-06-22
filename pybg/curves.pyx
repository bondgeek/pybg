# distutils: language = c++
# distutils: libraries = QuantLib

from libcpp.map cimport map
from libcpp.string cimport string

from cython.operator cimport dereference as deref

from pybg.quantlib.handle cimport shared_ptr

cimport _curves
cimport pybg.quantlib.time._date as _qldate
cimport pybg.quantlib.time.date as qldate
    
cdef public enum RateHelperType:
    DEPO    = _curves.DEPO
    FRA     = _curves.FRA
    FUT     = _curves.FUT
    SWAP    = _curves.SWAP

cdef class RateHelperCurve:
    """Rate Helper Curve
    
    """
    def __cinit__(self):
        self._thisptr = NULL

    def __dealloc__(self):
        if self._thisptr is not NULL:
            del self._thisptr
            
    def __init__(self, tenor="3M"):
        
        cdef char* tnr = tenor
        
        self._thisptr = new shared_ptr[_curves.RateHelperCurve]( \
            new _curves.RateHelperCurve(_curves.USDLiborCurve(<string>tnr))
            )

    dbg = """
    def update(self, evaldate, depos, swaps, fixingdays=-1):
    
        cdef qldate.Date qdate_ref = qldate.Date.from_datetime(evaldate)
        cdef _qldate.Date* date_ref = <_qldate.Date*>qdate_ref._thisptr.get()
        
        cdef _curves.CurveMap depocurve
        cdef _curves.CurveMap swapcurve
        
        cdef char* tnr
        cdef Rate value
        for t, value in depos.items():
            tnr = t
            depocurve[<string>tnr] = value
            
        for t, value in swaps.items():
            tnr = t
            swapcurve[<string>tnr] = value
            
        self._thisptr.get().update(deref(date_ref), depocurve, swapcurve, fixingdays)
        """