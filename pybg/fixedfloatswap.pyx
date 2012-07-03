# distutils: language = c++
# distutils: libraries = QuantLib

include 'quantlib/types.pxi'

cimport _fixedfloatswap

cimport pybg.quantlib.time._date as _qldate
cimport pybg.quantlib.time.date as qldate

from cython.operator cimport dereference as deref

from pybg.quantlib.handle cimport shared_ptr

cimport pybg.quantlib.indexes._libor as _libor
from pybg.quantlib.time._period cimport Frequency as _Frequency
from pybg.quantlib.time._calendar cimport Calendar as _Calendar
from pybg.quantlib.time._calendar cimport BusinessDayConvention as _BusinessDayConvention
from pybg.ql cimport _pydate_from_qldate, _qldate_from_pydate

from pybg.quantlib.time.daycounter cimport DayCounter
from pybg.quantlib.time.daycounters.thirty360 import (
        Thirty360, EUROBONDBASIS, EUROPEAN
)
from pybg.quantlib.time.calendar import Following, ModifiedFollowing, Unadjusted

from pybg.quantlib.time.api import *

cimport pybg.quantlib._cashflow as _cashflow
cimport pybg.quantlib.cashflow as cashflow
cimport pybg.curves as curves
cimport pybg._curves as _curves

cdef public enum SwapPayType:
    FixedPayer    = _fixedfloatswap.FixedPayer
    FixedReceiver = _fixedfloatswap.FixedReceiver
    

cdef class USDLiborSwap:
    def __cinit__(self):
        self._thisptr = NULL

    def __dealloc__(self):
        if self._thisptr is not NULL:
            del self._thisptr
    
    def __init__(self,
            tenor,
            object settle, 
            object maturity, 
            Rate fixedRate, 
            SwapPayType payerType,
            Spread floating_spread=0.0,
            Real notional=1000000.0,
            # Fixed Leg
            fixedLegFrequency=Semiannual,
            DayCounter fixedLegDayCounter=Thirty360(EUROPEAN),
            fixedLegConvention=ModifiedFollowing,
            # floating leg 
            floatingLegFrequency=Quarterly,
            DayCounter floatingLegDayCounter=Actual360(),
            floatingLegConvention=ModifiedFollowing
            ):
        """Enter tenor as '3M'
        
        """

        cdef _fixedfloatswap.SwapType[ _libor.USDLibor] *swptype = \
            new _fixedfloatswap.SwapType[ _libor.USDLibor](
                <_Frequency>fixedLegFrequency,
                deref(fixedLegDayCounter._thisptr),
                <_BusinessDayConvention>fixedLegConvention,
                <_Frequency>floatingLegFrequency,
                deref(floatingLegDayCounter._thisptr),
                <_BusinessDayConvention>floatingLegConvention,
                _Calendar.TARGET()
                )
        
        self._thisptr = new shared_ptr[_fixedfloatswap.FixedFloatSwap]( \
            deref(swptype).create(
                _qldate_from_pydate(settle),
                _qldate_from_pydate(maturity),
                fixedRate,
                payerType,
                floating_spread,
                notional
                )
            )
    
    def setEngine(self, curves.RateHelperCurve crv):
        cdef _curves.RateHelperCurve _crv
        
        _crv = deref(crv._thisptr.get())
        
        self._thisptr.get().setEngine(_crv)
        
        
    # Inspectors
    #    SwapPayType payerType()
    #    Real        nominal()
    #    Rate        spread()
    #    Rate        fixedRate()
    #    Leg         fixedLeg()
    #    Leg         floatingLeg()
    property payerType:
        def __get__(self):
            cdef SwapPayType payType 
            cdef _fixedfloatswap.SwapPayType _payType
            
            _payType = self._thisptr.get().payerType()
            
            payType = <SwapPayType>_payType 
            
            return payType
    
    property nominal:
        def __get__(self):
            cdef Real _nominal 
            
            _nominal = self._thisptr.get().nominal()
            
            return _nominal
    
    property fixedRate:
        def __get__(self):
            cdef Real _fixedRate 
            
            _fixedRate = self._thisptr.get().fixedRate()
            
            return _fixedRate

    property spread:
        def __get__(self):
            cdef Real _spread 
            
            _spread = self._thisptr.get().spread()
            
            return _spread            

    property fixedLeg:
        def __get__(self):
            cdef _cashflow.Leg leg
            cdef object result 
            
            leg = self._thisptr.get().fixedLeg()
            
            result = cashflow.leg_items(leg)
            
            return result

    property floatingLeg:
        def __get__(self):
            cdef _cashflow.Leg leg
            cdef object result 
            
            leg = self._thisptr.get().floatingLeg()
            
            result = cashflow.leg_items(leg)
            
            return result        
    
    #Results
    #    Real 	fixedLegBPS() 
    #    Real 	fixedLegNPV() 
    #    Rate 	fairRate()
    #    Real 	floatingLegBPS()
    #    Real 	floatingLegNPV()
    #    Spread 	fairSpread()
    property fixedLegBPS:
        def __get__(self):
            cdef Real result 
            
            result = self._thisptr.get().fixedLegBPS()
            
            return result

    property fixedLegNPV:
        def __get__(self):
            cdef Real result 
            
            result = self._thisptr.get().fixedLegNPV()
            
            return result
            
    property fairRate:
        def __get__(self):
            cdef Rate result 
            
            result = self._thisptr.get().fairRate()
            
            return result

    property fairSpread:
        def __get__(self):
            cdef Spread result 
            
            result = self._thisptr.get().fairSpread()
            
            return result
            
    property floatingLegBPS:
        def __get__(self):
            cdef Real result 
            
            result = self._thisptr.get().floatingLegBPS()
            
            return result 
                       
    property floatingLegNPV:
        def __get__(self):
            cdef Real result 
            
            result = self._thisptr.get().floatingLegNPV()
            
            return result
