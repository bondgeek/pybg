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
                 DayCounter daycounter=ActualActual(Bond),
                 frequency=Semiannual,
                 Real redemption=100.0,
                 Real faceamount=100.0,
                 accrualConvention=Unadjusted,
                 paymentConvention=Unadjusted
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
        cdef _curves.RateHelperCurve _crv
        
        _crv = deref(crv._thisptr.get())
        
        self._thisptr.get().setEngine(_crv)
        
    
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
