# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

from math import floor

from libcpp cimport bool

from pybg.settings import TaxRates

cimport pybg.quantlib.time._date as _qldate
cimport pybg.quantlib.time.date as qldate

from cython.operator cimport dereference as deref
from pybg.quantlib.handle cimport shared_ptr

from pybg.quantlib.time._period cimport Frequency as _Frequency
from pybg.quantlib.time._calendar cimport Calendar as _Calendar
from pybg.quantlib.time._daycounter cimport DayCounter as _DayCounter
from pybg.quantlib.time._calendar cimport BusinessDayConvention as _BusinessDayConvention

from pybg.ql cimport _pydate_from_qldate, _qldate_from_pydate
from pybg.settings import (
    get_eval_date, set_eval_date, PySettings, parse_date
    )
cimport pybg._curves as _curves
cimport pybg.curves as curves

import pybg.instruments.bulletbond as BB

from pybg.quantlib.time.daycounter cimport DayCounter
from pybg.quantlib.time.calendar cimport Calendar

cimport pybg.quantlib._cashflow as _cashflow
cimport pybg.quantlib.cashflow as cashflow

from pybg.enums import (
    DayCounters, Frequencies, BusinessDayConventions, Calendars
    )

cdef class CallBond:
    
    def __init__(self,
                 Rate coupon,
                 object maturity,
                 object first_calldate,
                 Real first_callprice,
                 object par_calldate,
                 object issue_date,
                 object oid_yield=None,
                 Calendar calendar=None,
                 int settlementDays=3,
                 DayCounter daycounter=None,
                 frequency=Frequencies.Semiannual,
                 callfrequency=Frequencies.Semiannual,
                 Real redemption=100.0,
                 Real faceamount=100.0,
                 accrualConvention=BusinessDayConventions.Unadjusted,
                 paymentConvention=BusinessDayConventions.Unadjusted,
                 object evaldate=None
                 ):
        '''initialize callable bond
        
        '''
        
        maturity, first_calldate, par_calldate, issue_date = \
            map(parse_date, 
                (maturity, first_calldate, par_calldate, issue_date))
        
        #TODO: default values for call dates, issue_date
        BondBase.__init__(self, 
                 evaldate,
                 coupon,
                 maturity,
                 issue_date=issue_date,
                 first_calldate=first_calldate,
                 first_callprice=first_callprice,
                 par_calldate=par_calldate,
                 callfrequency=callfrequency,
                 oid_yield=oid_yield
                 ) 
        
        if not daycounter:
            daycounter = DayCounters.ActualActual(DayCounters.Bond)
        
        if not calendar:
            calendar = Calendars.UnitedStates( \
                                            Calendars.GOVERNMENTBOND)
            
        self._thisptr = new shared_ptr[_instrumentbases.BondBase]( \
            new _callbond.CallBond(
                       coupon,
                       _qldate_from_pydate(maturity),
                       _qldate_from_pydate(first_calldate),
                       first_callprice,
                       _qldate_from_pydate(par_calldate),
                       _qldate_from_pydate(issue_date),
                       deref(calendar._thisptr), 
                       settlementDays, 
                       deref(daycounter._thisptr),
                       <_Frequency>frequency,
                       <_Frequency>callfrequency,
                       redemption, 
                       faceamount, 
                       <_BusinessDayConvention>accrualConvention, 
                       <_BusinessDayConvention>paymentConvention, 
                       _qldate_from_pydate(self.evalDate)
                       )
             )
        
    property spread:
        def __get__(self):
            cdef Real sprd
            sprd = (<_callbond.CallBond *>self._thisptr.get()).getSpread()
            return sprd
        
        def __set__(self, Real sprd):
           (<_callbond.CallBond *>self._thisptr.get()).setSpread(sprd)
    
    property reversionParameter:
        def __get__(self):
            cdef Real value
            value = (<_callbond.CallBond *>self._thisptr.get()).reversionParameter()

    property sigma:
        def __get__(self):
            cdef Real value
            value = (<_callbond.CallBond *>self._thisptr.get()).sigma()
    
    property lognormal:
        def __get__(self):
            cdef bool value
            value = (<_callbond.CallBond *>self._thisptr.get()).lognormal()
    
    property ytwFeature:
        def __get__(self):
            cdef int n 
            
            n = (<_callbond.CallBond *>self._thisptr.get()).ytwFeature()
            return n
            
    property settlementDate:
        def __get__(self):
            cdef _qldate.Date dt
            cdef object result 
            
            dt = (<_callbond.CallBond *>self._thisptr.get()).settlementDate()
            
            result = _pydate_from_qldate(dt)
            return result

        def __set__(self, object pydate):
            cdef _qldate.Date dt = _qldate_from_pydate(pydate)
            
            dt = (<_callbond.CallBond *>self._thisptr.get()).settlementDate(dt)

    property cashflows:
        def __get__(self):
            cdef _cashflow.Leg leg
            cdef object result 
            
            leg = (<_callbond.CallBond *>self._thisptr.get()).cashflows()
            
            result = cashflow.leg_items(leg)
            return result
            
    property redemptions:
        def __get__(self):
            cdef _cashflow.Leg leg
            cdef object result 
            
            leg = (<_callbond.CallBond *>self._thisptr.get()).redemptions()
            
            result = cashflow.leg_items(leg)
            return result 
            
    # Bond Math
    def toYTM(self, bondprice=None, redemption=100.0):
        cdef double yld
        
        if not bondprice:
            yld = (<_callbond.CallBond *>self._thisptr.get()).toYTM()
        else:
            yld = (<_callbond.CallBond *>self._thisptr.get()).toYTM(bondprice, 
                                                                    redemption)
        
        return yld
    
    def ytmToPrice(self, bondyield):
        cdef double prx
        
        prx = (<_callbond.CallBond *>self._thisptr.get()).ytmToPrice(bondyield)

        return prx 
                   
    
    def toATY(self, bondprice, oid=None, ptsyear=None, 
                  capgains=None, ordinc=None):
            
        _qtax = self.qtax(oid, ptsyear, capgains, ordinc)
        
        if bondprice <= _qtax["rv"]:
            amd = max(_qtax["rv"] - bondprice, 0)
            
            if amd < _qtax["discount"]:
                taxRate = _qtax["cg_rate"] 
            else:
                taxRate = _qtax["oi_rate"]
                
            rval = 100 - amd * taxRate
            aty = self.toYTM(bondprice, redemption=rval)

        else:
            aty = self.toYield(bondprice)
                        
        return aty
    
    def atyToPrice(self, atyield, oid=None, ptsyear=None, 
                  capgains=None, ordinc=None):
        '''
        Calculate price from after-tax yield
        '''
        
        _qtax = self.qtax(oid, ptsyear, capgains, ordinc)
                
        atPrice = self.ytmToPrice(atyield)
        
        if atPrice < _qtax["rv"]:
            amd = _qtax["rv"] - atPrice
            taxpv = BB.BulletBond(
                                  0.,
                                  self.maturity,
                                  self.issue_date,
                                  self.calendar,
                                  self.settlementDays,
                                  self.dayCounter,
                                  self.frequency,
                                  100.0,
                                  100.0,
                                  BusinessDayConventions.Unadjusted,
                                  BusinessDayConventions.Unadjusted,
                                  get_eval_date() ).toPrice(atyield)
                                       
            if amd < _qtax["discount"]:
                taxRate = _qtax['cg_rate']  
            else:
                taxRate = _qtax['oi_rate']
                
            taxHit = taxRate * taxpv * amd/(100. - taxpv * taxRate)
            atPrice -= taxHit
            
        return atPrice
        

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
        
        (<_callbond.CallBond *>self._thisptr.get()).oasEngine(_crv, 
                                                              a, 
                                                              sigma, 
                                                              spread, 
                                                              <bool>lognormal)


    def oasValue(self, 
                 Real spread, 
                 Real sigma, 
                 Real a=0.0,
                 int  lognormal=True):
        '''Calculates bond value from option adjusted spread
        
        '''
        cdef Real value
        value = (<_callbond.CallBond *>self._thisptr.get()).oasValue(spread, 
                                                                     sigma, 
                                                                     a, 
                                                                     <bool>lognormal)
        
        return value
    
    def oas(self,
            Real bondprice,
            Real sigma,
            Real a=0.0,
            int  lognormal=True):
        cdef Real value
        value = (<_callbond.CallBond *>self._thisptr.get()).oas(bondprice, 
                                                                sigma, 
                                                                a, 
                                                                <bool>lognormal)
        
        return value
    
    def oasImpliedVol(self,
                      Real bondprice,
                      Real spread,
                      Real a=0.0,
                      int  lognormal=True):
        cdef Real value
        value = (<_callbond.CallBond *>self._thisptr.get()).oasImpliedVol(
                                                              bondprice, 
                                                              spread, 
                                                              a, 
                                                              <bool>lognormal)
    
        return value
