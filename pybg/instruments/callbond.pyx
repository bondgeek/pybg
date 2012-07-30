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
from pybg.quantlib.time._calendar cimport BusinessDayConvention as _BusinessDayConvention
from pybg.ql cimport _pydate_from_qldate, _qldate_from_pydate

cimport pybg._curves as _curves
cimport pybg.curves as curves

from pybg.quantlib.time.daycounter cimport DayCounter
from pybg.quantlib.time.calendar cimport Calendar

from pybg.quantlib.time.daycounters.thirty360 import (
        Thirty360, EUROBONDBASIS, EUROPEAN
)
from pybg.quantlib.time.daycounters.actual_actual import (
        Bond, Euro, Historical, ISDA, ISMA, ActualActual, Actual365
)
from pybg.quantlib.time.calendars.united_states import (
        UnitedStates, GOVERNMENTBOND, NYSE, SETTLEMENT, NERC
)

from pybg.quantlib.time.calendar import (
    Following, ModifiedFollowing, Unadjusted
)

from pybg.quantlib.time.date import (
    Annual, Semiannual, Quarterly, Monthly, Weekly, Daily
)

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
                 Calendar calendar,
                 int settlementDays=3,
                 DayCounter daycounter=ActualActual(Bond),
                 frequency=Semiannual,
                 callfrequency=Semiannual,
                 Real redemption=100.0,
                 Real faceamount=100.0,
                 accrualConvention=Unadjusted,
                 paymentConvention=Unadjusted
                 ):

        self._thisptr = new shared_ptr[_callbond.CallBond]( \
            new _callbond.CallBond(
                       coupon,
                       _qldate_from_pydate( maturity ),
                       _qldate_from_pydate( calldate ),
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
                       )
             )
    
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
        
    # Bond Math
    def toPrice(self, Rate bondyield=None):
        cdef double prx
        
        if not bondyield:
            prx = self._thisptr.get().toPrice()
        else:
            prx = self._thisptr.get().toPrice(bondyield)
        
        return prx 
        
    def toYield(self, Real bondprice=None):
        cdef double yld
        
        if not bondprice:
            yld = self._thisptr.get().toYield()
        else:
            yld = self._thisptr.get().toYield(bondprice)
        
        return yld
         
    def toYTM(self, Real bondprice=None):
        cdef double yld
        
        if not bondprice:
            yld = self._thisptr.get().toYTM()
        else:
            yld = self._thisptr.get().toYTM(bondprice)
        
        return yld
    
    def ytmToPrice(self, Real bondyield):
        cdef double prx
        
        prx = self._thisptr.get().ytmToPrice(bondyield)

        return prx 
         
         
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
