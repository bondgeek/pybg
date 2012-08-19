# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

from math import floor

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

import pybg.enums as enums
import pybg.instruments.bulletbond as BB

from pybg.quantlib.time.daycounter cimport DayCounter
from pybg.quantlib.time.calendar cimport Calendar

cimport pybg.quantlib._cashflow as _cashflow
cimport pybg.quantlib.cashflow as cashflow

cdef class CallBond:
    def __cinit__(self):
        self._thisptr = NULL

    def __dealloc__(self):
        if self._thisptr is not NULL:
            del self._thisptr
    
    def __init__(self,
                 Rate coupon,
                 object maturity,
                 object calldate,
                 Real callprice,
                 object issue_date,
                 Calendar calendar=None,
                 int settlementDays=3,
                 DayCounter daycounter=None,
                 frequency=enums.Frequencies.Semiannual,
                 callfrequency=enums.Frequencies.Semiannual,
                 Real redemption=100.0,
                 Real faceamount=100.0,
                 accrualConvention=enums.BusinessDayConventions.Unadjusted,
                 paymentConvention=enums.BusinessDayConventions.Unadjusted,
                 object evaldate=None
                 ):

        if not evaldate:
            evaldate = get_eval_date()
        
        if not daycounter:
            daycounter = enums.DayCounters.ActualActual(enums.DayCounters.Bond)
        
        if not calendar:
            calendar = enums.Calendars.UnitedStates(enums.Calendars.GOVERNMENTBOND)
            
        self._coupon = <Real>coupon
        self._settlementDays = settlementDays
        
        self._thisptr = new shared_ptr[_callbond.CallBond]( \
            new _callbond.CallBond(
                       coupon,
                       _qldate_from_pydate(maturity),
                       _qldate_from_pydate(calldate),
                       callprice,
                       _qldate_from_pydate( issue_date),
                       deref(calendar._thisptr), 
                       settlementDays, 
                       deref(daycounter._thisptr),
                       <_Frequency>frequency,
                       <_Frequency>callfrequency,
                       redemption, 
                       faceamount, 
                       <_BusinessDayConvention>accrualConvention, 
                       <_BusinessDayConvention>paymentConvention, 
                       _qldate_from_pydate(evaldate)
                       )
             )

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
    
    property coupon:
        def __get__(self):
            cdef Real cpn
            cpn = self._coupon
            return cpn
        
    property spread:
        def __get__(self):
            cdef Real sprd
            sprd = self._thisptr.get().getSpread()
            return sprd
        
        def __set__(self, Real sprd):
            self._thisptr.get().setSpread(sprd)
    
    property reversionParameter:
        def __get__(self):
            cdef Real value
            value = self._thisptr.get().reversionParameter()

    property sigma:
        def __get__(self):
            cdef Real value
            value = self._thisptr.get().sigma()
    
    property lognormal:
        def __get__(self):
            cdef bool value
            value = self._thisptr.get().lognormal()
    
                
    property redemptions:
        def __get__(self):
            cdef _cashflow.Leg leg
            cdef object result 
            
            leg = self._thisptr.get().redemptions()
            
            result = cashflow.leg_items(leg)
            
            return result 
    
    property ytwFeature:
        def __get__(self):
            cdef int n 
            
            n = self._thisptr.get().ytwFeature()
            return n
            
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

    property settlementDays:
        def __get__(self):
            return self._settlementDays
    
    property maturity:
        def __get__(self):
            cdef _qldate.Date mty
            cdef object result 
            
            mty = self._thisptr.get().maturityDate()
            
            result = _pydate_from_qldate(mty)
            return result 
            
    property calendar:
        def __get__(self):
            cdef _Calendar dc
            cdef object result 
            
            dc = self._thisptr.get().calendar()
            
            return enums.Calendars()[dc.name().c_str()]

    property dayCounter:
        def __get__(self):
            cdef _DayCounter dc
            cdef object result 
            
            dc = self._thisptr.get().dayCounter()
            
            return enums.DayCounters()[dc.name().c_str()]

    property frequency:
        def __get__(self):
            cdef _Frequency freq 
            cdef object result 
            
            freq = self._thisptr.get().frequency()
            
            return freq
            
    property issueDate:
        def __get__(self):
            cdef _qldate.Date dt
            cdef object result 
            
            dt = self._thisptr.get().issueDate()
            
            result = _pydate_from_qldate(dt)
            return result
            
    # Bond Math
    def toPrice(self, bondyield=None):
        cdef double prx
        
        if not bondyield:
            prx = self._thisptr.get().toPrice()
        else:
            prx = self._thisptr.get().toPrice(bondyield)
        
        return prx 
        
    def toYield(self, bondprice=None):
        cdef double yld
        
        if not bondprice:
            yld = self._thisptr.get().toYield()
        else:
            yld = self._thisptr.get().toYield(bondprice)
        
        return yld
         
    def toYTM(self, bondprice=None, redemption=100.0):
        cdef double yld
        
        if not bondprice:
            yld = self._thisptr.get().toYTM()
        else:
            yld = self._thisptr.get().toYTM(bondprice, redemption)
        
        return yld
    
    def ytmToPrice(self, bondyield):
        cdef double prx
        
        prx = self._thisptr.get().ytmToPrice(bondyield)

        return prx 
                   
    def qtax(self, oid, ptsyear=0.25):
        """Calculate de minimus cut-off for market discount bonds
        
        """
        _qtax = {"ptsyear": ptsyear}    
        
        if oid > self.coupon:
            _qtax["oid"] = oid
            _qtax["rval"] = self.ytmToPrice(oid)
        else:
            _qtax["oid"] = self.coupon
            _qtax["rval"] = 100.0
        
        nyrs = enums.DayCounters.year_fraction(self.settlementDate,
                                               self.maturity)
                                                        
        _qtax["demin"] = float(floor(nyrs)) * ptsyear
        
        _qtax["cut_off_yield"] = self.toYTM(_qtax["rval"] - _qtax["demin"])
            
        return _qtax
    
    def toATY(self, bondprice, capgains, ordinc, oid=None, ptsyear=0.25):
            
        _qtax = self.qtax(oid, ptsyear)
        
        if bondprice <= _qtax["rval"]:
            amd = max(_qtax["rval"] - bondprice, 0)
            taxRate = capgains if amd < _qtax["demin"] else ordinc
            rval = 100 - amd * taxRate
            aty = self.toYTM(bondprice, redemption=rval)

        else:
            aty = self.toYield(bondprice)
                        
        return aty
    
    def atyToPrice(self, atyield, capgains, ordinc, oid=None, ptsyear=0.25):
        
        _qtax = self.qtax(oid, ptsyear)
                
        atPrice = self.ytmToPrice(atyield)
        
        if atPrice < _qtax["rval"]:
            amd = _qtax["rval"] - atPrice
            taxpv = BB.BulletBond(
                                  0.,
                                  self.maturity,
                                  self.issueDate,
                                  self.calendar,
                                  self.settlementDays,
                                  self.daycounter,
                                  self.frequency,
                                  100.0,
                                  100.0,
                                  enums.BusinessDayConventions.Unadjusted,
                                  enums.BusinessDayConventions.Unadjusted,
                                  get_eval_date() ).toPrice(atyield)
                                               
            taxRate = capgains if amd < _qtax["demin"] else ordinc
            taxHit = taxRate * taxpv * amd/(100. - taxpv * taxRate)
            atPrice -= taxHit
            
        return atPrice
        
    
    def setEngine(self, 
                  curves.RateHelperCurve crv,
                  Real                   a,
                  Real                   sigma,
                  int                    lognormal=True):
        cdef _curves.RateHelperCurve _crv
        
        _crv = deref(crv._thisptr.get())
        
        self._thisptr.get().setEngine(_crv, a, sigma, <bool>lognormal)
        

    def oasEngine(self,
                  curves.RateHelperCurve crv,
                  Real a,
                  Real sigma,
                  Real spread=0.0,
                  int  lognormal=True):
        '''set price engine for oas calcs
        
        lognormal:  True, uses BlackKarasinski
                    False, uses Hull-White
        
        '''
        cdef _curves.RateHelperCurve _crv
        
        _crv = deref(crv._thisptr.get())
        
        self._thisptr.get().oasEngine(_crv, a, sigma, spread, <bool>lognormal)


    def oasValue(self, 
                 Real spread, 
                 Real sigma, 
                 Real a=0.0,
                 int  lognormal=True):
        '''Calculates bond value from option adjusted spread
        
        '''
        cdef Real value
        value = self._thisptr.get().oasValue(spread, sigma, a, <bool>lognormal)
        
        return value
    
    def oas(self,
            Real bondprice,
            Real sigma,
            Real a=0.0,
            int  lognormal=True):
        cdef Real value
        value = self._thisptr.get().oas(bondprice, sigma, a, <bool>lognormal)
        
        return value
    
    def oasImpliedVol(self,
                      Real bondprice,
                      Real spread,
                      Real a=0.0,
                      int  lognormal=True):
        cdef Real value
        value = self._thisptr.get().oasImpliedVol(bondprice, 
                                                  spread, 
                                                  a, 
                                                  <bool>lognormal)
    
        return value
