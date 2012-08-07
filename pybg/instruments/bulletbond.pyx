# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

include '../quantlib/types.pxi'

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

cimport pybg._curves as _curves
cimport pybg.curves as curves

from pybg.quantlib.time.daycounter cimport DayCounter
from pybg.quantlib.time.calendar cimport Calendar

cimport pybg.quantlib._cashflow as _cashflow
cimport pybg.quantlib.cashflow as cashflow

from pybg.enums import (
    DayCounters, Frequencies, BusinessDayConventions, Calendars
    )

cdef class BulletBond:
    def __cinit__(self):
        self._thisptr = NULL

    def __dealloc__(self):
        if self._thisptr is not NULL:
            del self._thisptr
    
    def __init__(self,
                 Rate coupon,
                 object maturity,
                 object issue_date,
                 Calendar calendar,
                 int settlementDays=3,
                 DayCounter daycounter=DayCounters.ActualActual(DayCounters.Bond),
                 frequency=Frequencies.Semiannual,
                 Real redemption=100.0,
                 Real faceamount=100.0,
                 accrualConvention=BusinessDayConventions.Unadjusted,
                 paymentConvention=BusinessDayConventions.Unadjusted
                 ):
        
        self._thisptr = new shared_ptr[_bulletbond.BulletBond]( \
            new _bulletbond.BulletBond(
                       coupon,
                       _qldate_from_pydate( maturity ),
                       _qldate_from_pydate( issue_date),
                       deref(calendar._thisptr), 
                       settlementDays, 
                       deref(daycounter._thisptr),
                       <_Frequency>frequency,
                       redemption, 
                       faceamount, 
                       <_BusinessDayConvention>accrualConvention, 
                       <_BusinessDayConvention>paymentConvention, 
                       ))
        
    def setEngine(self, curves.RateHelperCurve crv):
        cdef _curves.CurveBase _crv  = <_curves.CurveBase>deref(crv._thisptr.get()) 
        print("\nsetEngine(py): %s " % crv.curveDate)
        print("\n_crv.curveDate: %s " % _pydate_from_qldate(_crv.curveDate()))
        
        print("\n_crv.curveDate2: %s " % _pydate_from_qldate(crv._thisptr.get().curveDate()))
        print("\nsetEngine(py)2: %s " % crv.curveDate)
        self._thisptr.get().setEngine(_crv)
        
    
    def toPrice(self, bondyield=None):
        if bondyield:
            return self._thisptr.get().toPrice(bondyield)
        else:
            return self._thisptr.get().toPrice()
            
    def toYield(self, bondprice=None):
        if bondprice:
            print("in toYield")
            return self._thisptr.get().toYield(bondprice)
        else:
            return self._thisptr.get().toYield()
      
    # Inspectors
    #    _QLDate maturityDate()
    #    _QLDate settlementDate()
    #    _QLDate issueDate()
    #    DayCounter dayCounter()
    #    Frequency frequency()
    #    Natural settlementDays()
    #    Calendar calendar()
    #    Leg redemptions()
    #    Leg cashflows()
    
    property redemptions:
        def __get__(self):
            cdef _cashflow.Leg leg
            cdef object result 
            
            leg = self._thisptr.get().redemptions()
            
            result = cashflow.leg_items(leg)
            return result 
    
    property cashflows:
        def __get__(self):
            cdef _cashflow.Leg leg
            cdef object result 
            
            leg = self._thisptr.get().cashflows()
            
            result = cashflow.leg_items(leg)
            return result 

    property calendar:
        def __get__(self):
            cdef _Calendar dc
            cdef object result 
            
            dc = self._thisptr.get().calendar()
            
            return Calendars()[dc.name().c_str()]

    property dayCounter:
        def __get__(self):
            cdef _DayCounter dc
            cdef object result 
            
            dc = self._thisptr.get().dayCounter()
            
            return DayCounters()[dc.name().c_str()]

    property issueDate:
        def __get__(self):
            cdef _qldate.Date dt
            cdef object result 
            
            dt = self._thisptr.get().issueDate()
            
            result = _pydate_from_qldate(dt)
            return result
            
    property settlementDate:
        def __get__(self):
            cdef _qldate.Date dt
            cdef object result 
            
            dt = self._thisptr.get().settlementDate()
            
            result = _pydate_from_qldate(dt)
            return result

        def __set__(self, object pydate):
            cdef _qldate.Date dt = _qldate_from_pydate(pydate)
            
            dt = self._thisptr.get().settlementDate(dt)

            
    property maturity:
        def __get__(self):
            cdef _qldate.Date mty
            cdef object result 
            
            mty = self._thisptr.get().maturityDate()
            
            result = _pydate_from_qldate(mty)
            return result 
