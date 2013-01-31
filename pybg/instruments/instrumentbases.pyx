# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

from libcpp cimport bool

from math import floor

cimport pybg.quantlib.time._date as _qldate
cimport pybg.quantlib.time.date as qldate

from cython.operator cimport dereference as deref
from pybg.quantlib.handle cimport shared_ptr

from pybg.quantlib.time._period cimport Frequency as _Frequency
from pybg.quantlib.time._calendar cimport Calendar as _Calendar
from pybg.quantlib.time._daycounter cimport DayCounter as _DayCounter
from pybg.quantlib.time._calendar cimport BusinessDayConvention as _BusinessDayConvention

from pybg.ql cimport _pydate_from_qldate, _qldate_from_pydate
from pybg.settings import get_eval_date, set_eval_date, PySettings

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
                 object evaldate,
                 Rate coupon,
                 object maturity,
                 object issue_date=None,
                 object first_calldate=None,
                 Real first_callprice=100.0,
                 object par_calldate=None,
                 callfrequency=Frequencies.Annual,
                 object sinkingfund=None,
                 sinkingfundFrequency=Frequencies.Annual,
                 object oid_yield=None
                 ):
                 
        self.evalDate = get_eval_date() if not evaldate else evaldate
                
        self._firstCallDate = first_calldate
        if not first_calldate:
            self._callable = False
            self._firstCallPrice = 0.
            self._parCallDate = None
        else:
            self._callable = True
            self._firstCallPrice = first_callprice
            self._parCallDate = par_calldate if par_calldate else maturity
            self._callfrequency = callfrequency
        
        self._issueDate = self.evalDate if not issue_date else issue_date
        self._coupon = coupon
        self._maturity = maturity
        self._sinkingfund = sinkingfund 
        self._sinker = False if not sinkingfund else True
        self._sinkingfundFrequency = sinkingfundFrequency
        self._oidYield = coupon if not oid_yield else oid_yield
        
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
    
    def accrued(self):
        return self._thisptr.get().accrued()
        
    def qtax(self, oid=None, demin_ptsyear=None, capgains=None, ordinc=None):
        self._qtax = {}
        
        if not oid:
            oid = self._oidYield
        
        if not capgains:
            self._qtax['cg_rate'] = PySettings.instance.taxrates.cap_gains
        else:
            self._qtax['cg_rate'] = capgains
        
        if not ordinc:
            self._qtax['oi_rate'] = PySettings.instance.taxrates.ord_income
        else:
            self._qtax['oi_rate'] = ordinc
            
        if not demin_ptsyear:
            demin_ptsyear = PySettings.instance.taxrates.demin_ptsyear

        self._qtax['oid'] = max(oid, self.coupon)
        self._qtax['rv'] = min(100.0, self.toPrice(oid))
        
        nyears = DayCounters.year_fraction(self.settlementDate,
                                           self.maturity)

        self._qtax['discount'] = float(floor(nyears)) * demin_ptsyear
        self._qtax['price'] = self._qtax['rv'] - self._qtax['discount']
        try:
            self._qtax['yield'] = self.toYield(self._qtax['price'])
            
        except:
            self._qtax['yield'] = self.coupon
            
        return self._qtax
        
    # derived classes will supersede these
    property settlementDate:
        def __get__(self):
            return Calendars.advance(self.evalDate, self.settlementDays)
                
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
    
    property issue_date:
        def __get__(self):
            return self._issueDate
        
    property maturity:
        def __get__(self):
            return self._maturity 
            
    property coupon:
        def __get__(self):
            return self._coupon
            
    property sinker:
        def __get__(self):
            if self._sinker:
                return (self._sinkingfund, self._sinkingfundFrequency)
            else:
                return None
    
    property callfeature:
        def __get__(self):
            if self._callable:
                return (self._firstCallDate, 
                        self._firstCallPrice, 
                        self._parCallDate,
                        self._callfrequency)
            else:
                return None
                
    

            