# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

include '../quantlib/types.pxi'

cimport pybg.instruments._fixedfloatswap as _fixedfloatswap

cimport pybg.quantlib.time._date as _qldate
cimport pybg.quantlib.time.date as qldate

from cython.operator cimport dereference as deref

from pybg.quantlib.handle cimport shared_ptr

cimport pybg.quantlib.indexes._libor as _libor
cimport pybg.quantlib.indexes._euribor as _euribor

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
from pybg._curves cimport (
    RateHelperCurve as _RateHelperCurve, CurveBase as _CurveBase
    )

cdef public enum SwapPayType:
    FixedPayer    = _fixedfloatswap.FixedPayer
    FixedReceiver = _fixedfloatswap.FixedReceiver

class SwapPayerType(object):
    FixedPayer    = _fixedfloatswap.FixedPayer
    FixedReceiver = _fixedfloatswap.FixedReceiver

cdef class LiborSwap:
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
            Spread floating_spread,
            Real notional,
            # Fixed Leg
            fixedLegFrequency,
            DayCounter fixedLegDayCounter,
            fixedLegConvention,
            # floating leg 
            floatingLegFrequency,
            DayCounter floatingLegDayCounter,
            floatingLegConvention
            ):
        """Enter tenor as '3M'
        
        """

        print("Abstract Base Class")
        
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
    #    Real   NPV()
    #    Real   fixedLegBPS() 
    #    Real   fixedLegNPV() 
    #    Rate   fairRate()
    #    Real   floatingLegBPS()
    #    Real   floatingLegNPV()
    #    Spread fairSpread()
    property NPV:
        def __get__(self):
            cdef Real result 
            
            result = self._thisptr.get().NPV()
            
            return result

            
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

cdef class USDLiborSwap(LiborSwap):
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
                    
        self._swaptype = \
            new _fixedfloatswap.SwapType[ _libor.USDLibor ](
                <_Frequency>fixedLegFrequency,
                deref(fixedLegDayCounter._thisptr),
                <_BusinessDayConvention>fixedLegConvention,
                <_Frequency>floatingLegFrequency,
                deref(floatingLegDayCounter._thisptr),
                <_BusinessDayConvention>floatingLegConvention,
                _Calendar.TARGET()
                )
                
        self._thisptr = new shared_ptr[_fixedfloatswap.FixedFloatSwap]( \
            deref(self._swaptype).create(
                _qldate_from_pydate(settle),
                _qldate_from_pydate(maturity),
                fixedRate,
                payerType,
                floating_spread,
                notional
                )
            )     
    
    def setEngine(self, curves.RateHelperCurve crv):
        #cdef _RateHelperCurve _crv
        cdef _CurveBase _crv
        
        _crv = <_CurveBase>deref(crv._thisptr.get())
        
        deref(self._swaptype).linkIndex(_crv)
        self._thisptr.get().setEngine(_crv)
              
                                      
cdef class EuriborSwap(LiborSwap):    
    def __init__(self,
            tenor,
            object settle, 
            object maturity, 
            Rate fixedRate, 
            SwapPayType payerType,
            Spread floating_spread=0.0,
            Real notional=1000000.0,
            # Fixed Leg
            fixedLegFrequency=Annual,
            DayCounter fixedLegDayCounter=Thirty360(EUROPEAN),
            fixedLegConvention=Unadjusted,
            # floating leg 
            floatingLegFrequency=Semiannual,
            DayCounter floatingLegDayCounter=Actual360(),
            floatingLegConvention=ModifiedFollowing
            ):
            
        self._swaptype = \
            new _fixedfloatswap.SwapType[ _euribor.Euribor ](
                <_Frequency>fixedLegFrequency,
                deref(fixedLegDayCounter._thisptr),
                <_BusinessDayConvention>fixedLegConvention,
                <_Frequency>floatingLegFrequency,
                deref(floatingLegDayCounter._thisptr),
                <_BusinessDayConvention>floatingLegConvention,
                _Calendar.TARGET()
                )
                
        self._thisptr = new shared_ptr[_fixedfloatswap.FixedFloatSwap]( \
            deref(self._swaptype).create(
                _qldate_from_pydate(settle),
                _qldate_from_pydate(maturity),
                fixedRate,
                payerType,
                floating_spread,
                notional
                )
            )

    def setEngine(self, curves.RateHelperCurve crv):
        #cdef _RateHelperCurve _crv
        cdef _CurveBase _crv
        
        _crv = <_CurveBase>deref(crv._thisptr.get())
        
        deref(self._swaptype).linkIndex(_crv)
        self._thisptr.get().setEngine(_crv)