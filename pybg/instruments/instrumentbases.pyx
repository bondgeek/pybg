# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

from libcpp cimport bool

cimport pybg.quantlib.time._date as _qldate
cimport pybg.quantlib.time.date as qldate

from cython.operator cimport dereference as deref
from pybg.quantlib.handle cimport shared_ptr

from pybg.quantlib.time._period cimport Frequency as _Frequency
from pybg.quantlib.time._calendar cimport Calendar as _Calendar
from pybg.quantlib.time._daycounter cimport DayCounter as _DayCounter
from pybg.quantlib.time._calendar cimport BusinessDayConvention as _BusinessDayConvention

from pybg.ql cimport _pydate_from_qldate, _qldate_from_pydate
from pybg.ql import get_eval_date, set_eval_date

cimport pybg._curves as _curves
cimport pybg.curves as curves

from pybg.enums import (
    DayCounters, Frequencies, BusinessDayConventions, Calendars
    )

cdef class BondBase:
    def __cinit__(self):
        self._thisptr = NULL

    def __dealloc__(self):
        if self._thisptr is not NULL:
            del self._thisptr
    
    def __init__(self, 
                 Rate coupon,
                 object maturity,
                 object issue_date=None,
                 object calldate=None,
                 Real callprice=100.,
                 object parcalldate=None,
                 object sinkingfund=None,
                 sinkingfundFrequency=Frequencies.Annual
                 ):
        raise NotImplementedError("Bond Base is a virtual base class")
                
    def setEngine(self, crv, a=0., sigma=0., lognormal=True):
        cdef _curves.CurveBase _crv
        
        if type(crv) == curves.RateHelperCurve:
            _crv = <_curves.CurveBase>deref((<curves.RateHelperCurve>crv)._thisptr.get()) 
        if type(crv) == curves.BondCurve:
            _crv = <_curves.CurveBase>deref((<curves.BondCurve>crv)._thisptr.get()) 
        
        self._thisptr.get().setEngine(_crv, a, sigma, <bool>lognormal)

    
    def toPrice(self, bondyield=None):
        if bondyield:
            return self._thisptr.get().toPrice(bondyield)
        else:
            return self._thisptr.get().toPrice()
            
    def toYield(self, bondprice=None):
        if bondprice:
            return self._thisptr.get().toYield(bondprice)
        else:
            return self._thisptr.get().toYield()
      
    # Inspectors from BondBase
    property evalDate:
        def __get__(self):
            cdef _qldate.Date dt
            cdef object result 
            
            dt = self._thisptr.get().get_eval_date()
            
            result = _pydate_from_qldate(dt)
            return result
            
        def __set__(self, object evaldate):
            cdef _qldate.Date dt
            cdef object result 
            
            dt = _qldate_from_pydate(evaldate)
            
            self._thisptr.get().set_eval_date(dt)
        
    property calendar:
        def __get__(self):
            cdef _Calendar dc
            cdef object result 
            
            dc = self._thisptr.get().get_calendar()
            
            return Calendars()[dc.name().c_str()]

    property dayCounter:
        def __get__(self):
            cdef _DayCounter dc
            cdef object result 
            
            dc = self._thisptr.get().get_dayCounter()
            
            return DayCounters()[dc.name().c_str()]

    property frequency:
        def __get__(self):
            cdef _Frequency freq 
            cdef object result 
            
            freq = self._thisptr.get().get_frequency()
            
            return freq

    property settlementDays:
        def __get__(self):
            cdef Natural stlDays
            
            stlDays = self._thisptr.get().get_settlementDays()
            
            return int(stlDays)
    

            