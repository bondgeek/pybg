# distutils: language = c++
# distutils: libraries = QuantLib

include 'quantlib/types.pxi'

from libcpp.map cimport map
from libcpp.string cimport string

from cython.operator cimport dereference as deref

cimport _curves
cimport pybg.quantlib.time._date as _qldate
cimport pybg.quantlib.time.date as qldate

from pybg.quantlib.handle cimport shared_ptr

from pybg.quantlib.time.api import *

from pybg.ql cimport _pydate_from_qldate, _qldate_from_pydate
from pybg.ql import get_eval_date, set_eval_date

from datetime import date 

cdef public enum RateHelperType:
    DEPO    = _curves.DEPO
    FRA     = _curves.FRA
    FUT     = _curves.FUT
    SWAP    = _curves.SWAP


cdef _curves.CurveMap curveMap_from_dict(pycurve):
    cdef _curves.CurveMap   curve
        
    cdef char* tnr
    cdef Rate value
    for t, value in pycurve.items():
        t = t.upper()
        tnr = t
        curve[<string>tnr] = value
    return curve 


cdef class CurveBase:
    """Rate Helper Curve
    
    """
    def __cinit__(self):
        self._thisptr = NULL

    def __dealloc__(self):
        if self._thisptr is not NULL:
            del self._thisptr
            
    def __init__(self, CurveBase curvebase):
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

    def update(self, depos=None, futures=None, swaps=None, evaldate=None, fixingdays=-1):

        MSG_ARGS = "RateHelperCurve.update must have at least one curve"
        assert any((depos, futures, swaps)), MSG_ARGS
        
        cdef _curves.CurveMap   depocurve
        cdef _curves.CurveMap   futcurve
        cdef _curves.CurveMap   swapcurve
        
        #TODO: validate curve inputs
        #      check that tenors/future dates don't overlap
        if depos:
            depocurve = curveMap_from_dict(depos)

        if futures:
            futcurve = curveMap_from_dict(futures)
                
        if swaps:
            swapcurve = curveMap_from_dict(swaps)

        if not evaldate:
            evaldate = date.today()
        
        set_eval_date(evaldate)
        evaldate = get_eval_date()
        
        cdef _qldate.Date date_ref  = _qldate_from_pydate(evaldate)
        
        self._thisptr.get().update(depocurve, futcurve, swapcurve, date_ref,  fixingdays)
        
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
        key = key.upper()
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
        


        
