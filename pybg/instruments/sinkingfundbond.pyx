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

from pybg.quantlib.time.daycounter cimport DayCounter
from pybg.quantlib.time.calendar cimport Calendar

cimport pybg.quantlib._cashflow as _cashflow
cimport pybg.quantlib.cashflow as cashflow

from pybg.enums import (
    DayCounters, Frequencies, BusinessDayConventions, Calendars
    )

cdef class SinkingFundBond:    
    def __init__(self,
                 Rate coupon,
                 object maturity,
                 object sinkingfund,
                 sinkingfundFrequency=Frequencies.Annual,
                 object issue_date=None,
                 Calendar calendar=None,
                 int settlementDays=3,
                 DayCounter daycounter=DayCounters.ActualActual(DayCounters.Bond),
                 frequency=Frequencies.Semiannual,
                 Real redemption=100.0,
                 Real faceamount=100.0,
                 accrualConvention=BusinessDayConventions.Unadjusted,
                 paymentConvention=BusinessDayConventions.Unadjusted,
                 object evaldate=None
                 ):
        '''sinking fund bond
        
        '''
        
        BondBase.__init__(self, 
                 evaldate,
                 coupon,
                 maturity,
                 issue_date=issue_date,
                 sinkingfund=sinkingfund,
                 sinkingfundFrequency=sinkingfundFrequency
                 ) 
        
        if not daycounter:
            daycounter = DayCounters.ActualActual(DayCounters.Bond)
        
        if not calendar:
            calendar = Calendars.UnitedStates( \
                                            Calendars.GOVERNMENTBOND)
                                                     
        cdef vector[Real] sf 
        cdef Real sf_pmt
        for sf_pmt in sinkingfund:
            sf.push_back(sf_pmt)
                
        self._thisptr = new shared_ptr[_instrumentbases.BondBase]( \
            new _sinkingfundbond.SinkingFundBond(
                       coupon,
                       _qldate_from_pydate( maturity ),
                       sf,
                       <_Frequency>sinkingfundFrequency,
                       _qldate_from_pydate( issue_date ),
                       deref(calendar._thisptr), 
                       settlementDays, 
                       deref(daycounter._thisptr),
                       <_Frequency>frequency,
                       redemption, 
                       faceamount, 
                       <_BusinessDayConvention>accrualConvention, 
                       <_BusinessDayConvention>paymentConvention, 
                       _qldate_from_pydate(self.evalDate)
                       ))
        
        
    # Bond Inspectors
    #Inspectors from FixedRateBond or BulletBond class 
    property settlementDate:
        def __get__(self):
            cdef _qldate.Date dt
            cdef object result 
            
            dt = (<_sinkingfundbond.SinkingFundBond *>self._thisptr.get()).settlementDate()
            
            result = _pydate_from_qldate(dt)
            return result

        def __set__(self, object pydate):
            cdef _qldate.Date dt = _qldate_from_pydate(pydate)
            
            dt = (<_sinkingfundbond.SinkingFundBond *>self._thisptr.get()).settlementDate(dt)
            
        
    property cashflows:
        def __get__(self):
            cdef _cashflow.Leg leg
            cdef object result 
            
            leg = (<_sinkingfundbond.SinkingFundBond *>self._thisptr.get()).cashflows()
            
            result = cashflow.leg_items(leg)
            return result
            
    property redemptions:
        def __get__(self):
            cdef _cashflow.Leg leg
            cdef object result 
            
            leg = (<_sinkingfundbond.SinkingFundBond *>self._thisptr.get()).redemptions()
            
            result = cashflow.leg_items(leg)
            return result 
