# distutils: language = c++
# distutils: libraries = QuantLib

include 'quantlib/types.pxi'

from libcpp.map cimport map
from libcpp.string cimport string

from cython.operator cimport dereference as deref

cimport _curves
cimport pybg.quantlib.time._date as _qldate
cimport pybg.quantlib.time.date as qldate

from pybg.quantlib.time._period cimport Frequency as _Frequency
from pybg.quantlib.time._calendar cimport Calendar as _Calendar

from pybg.quantlib.handle cimport shared_ptr

from pybg.quantlib.time.api import *

from pybg.ql cimport _pydate_from_qldate, _qldate_from_pydate
from datetime import date 

cdef public enum RateHelperType:
    DEPO    = _curves.DEPO
    FRA     = _curves.FRA
    FUT     = _curves.FUT
    SWAP    = _curves.SWAP


cdef class CurveBase:
    """Rate Helper Curve
    
    """
    def __cinit__(self):
        self._thisptr = NULL

    def __dealloc__(self):
        if self._thisptr is not NULL:
            del self._thisptr
            
    def __init__(self):
        raise ValueError(
            'This is an abstract class.'
        )
            
                        
cdef class RateHelperCurve:
    """Rate Helper Curve
    
    """
    def __cinit__(self):
        self._thisptr = NULL

    def __dealloc__(self):
        if self._thisptr is not NULL:
            del self._thisptr

    def __init__(self, CurveBase curvebase):
        
        cdef _curves.CurveBase *_crvbase
        
        try:
            _crvbase = curvebase._thisptr.get()
            self._thisptr = new shared_ptr[_curves.RateHelperCurve]( \
                new _curves.RateHelperCurve(deref(_crvbase))
                )
        except:
            self._thisptr = new shared_ptr[_curves.RateHelperCurve]( \
                new _curves.RateHelperCurve()
                )


    def update(self, depos, swaps, evaldate=None, fixingdays=-1):

        cdef _curves.CurveMap   depocurve
        cdef _curves.CurveMap   swapcurve
        
        cdef char* tnr
        cdef Rate value
        for t, value in depos.items():
            tnr = t
            depocurve[<string>tnr] = value
            
        for t, value in swaps.items():
            tnr = t
            swapcurve[<string>tnr] = value

        if not evaldate:
            evaldate = date.today()

        cdef _qldate.Date date_ref  = _qldate_from_pydate(evaldate)
        
        self._thisptr.get().update(date_ref, depocurve, swapcurve, fixingdays)
        
        return self.referenceDate

    property curveDate:
        def __get__(self):
            cdef _qldate.Date _refdate = self._thisptr.get().curvedate()
            return _pydate_from_qldate(_refdate)
    
    property referenceDate:
        def __get__(self):
            cdef _qldate.Date _refdate = self._thisptr.get().referenceDate()
            return _pydate_from_qldate(_refdate)
    
    property maxDate:
        def __get__(self):
            cdef _qldate.Date _qdate_ref = self._thisptr.get().maxDate()
            return _pydate_from_qldate(_qdate_ref)
    
    property fixingdays:
        def __get__(self):
            cdef int fixdays_ref = self._thisptr.get().fixingDays()
            return fixdays_ref
    
    # Curve functions
    def tenorquote(self, key):
        cdef char* tnr = key
        cdef Rate rate = self._thisptr.get().tenorquote(<string>tnr)
        return rate
        
    def discount(self, ref):
        cdef double         yrs 
        cdef _qldate.Date   _date_ref
        cdef double discfactor
        
        if type(ref) == type(date.today()):
            _date_ref = _qldate_from_pydate(ref)
            discfactor = self._thisptr.get().discount(_date_ref, True)
            
        else:
            try:
                yrs = <double>ref
            except:
                yrs = 0.0
                
            discfactor = self._thisptr.get().discount(yrs, True)
        
        return discfactor
        


        
