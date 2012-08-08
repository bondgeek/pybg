# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

include 'quantlib/types.pxi'
cimport pybg.version

from libcpp.map cimport map
from libcpp.string cimport string

from cython.operator cimport dereference as deref, preincrement as inc
from cython.operator cimport address

cimport pybg._curves as _curves
cimport pybg.quantlib.time._date as _qldate
cimport pybg.quantlib.time._period as _qlperiod
cimport pybg.quantlib.time.date as qldate

cimport pybg.quantlib.time._calendar as _calendar
from pybg.quantlib.time._period cimport Frequency as _Frequency
from pybg.quantlib.time._calendar cimport BusinessDayConvention as _BusinessDayConvention

from pybg.quantlib.handle cimport shared_ptr

from pybg.ql cimport _pydate_from_qldate, _qldate_from_pydate
from pybg.ql import get_eval_date, set_eval_date

# TODO: don't want to use import *
#from pybg.quantlib.time.api import *
from pybg.enums import TimeUnits, Calendars

cimport pybg.quantlib.time.calendar as calendar 
from pybg.quantlib.time.daycounter cimport DayCounter

from datetime import date 

cdef public enum RateHelperType:
    DEPO    = _curves.DEPO
    FRA     = _curves.FRA
    FUT     = _curves.FUT
    SWAP    = _curves.SWAP


cdef _curves.CurveMap curveMap_from_dict(pycurve):
    cdef _curves.CurveMap   curve
        
    cdef char* tnr
    cdef Rate value
    for t, value in pycurve.items():
        t = t.upper()
        tnr = t
        curve[<string>tnr] = value
    return curve 

cdef object dict_from_CurveMap(_curves.CurveMap crv):
    cdef map[string, Rate].iterator iter 
    
    pycurve = {}
    iter = crv.begin()
    while iter != crv.end():
        pycurve[deref(iter).first.c_str()] = deref(iter).second
        inc(iter)
        
    return pycurve
            
cdef class CurveBase:
    """Curve Base
    
    """
    def __cinit__(self):
        self._thisptr = NULL

    def __dealloc__(self):
        if self._thisptr is not NULL:
            del self._thisptr
            
    def __init__(self, 
                  calendar.Calendar     crv_calendar,
                  Integer               fixingDays,                  
                  DayCounter            depositDayCounter,
                  fixedRateFrequency,
                  fixedInstrumentConvention, 
                  DayCounter            fixedInstrumentDayCounter, 
                  DayCounter            termStructureDayCounter):
        '''
        Curve Base Class:
                  Calendar              calendar,
                  Integer               fixingDays,                  
                  DayCounter            depositDayCounter,
                  Frequency             fixedRateFrequency,
                  BusinessDayConvention fixedInstrumentConvention, 
                  DayCounter            fixedInstrumentDayCounter, 
                  DayCounter            termStructureDayCounter
                  
        '''
        # TODO:  give CurveBase the ability to set yieldTermStruct pointer, etc
        #        and you can use this
        self._thisptr = new shared_ptr[_curves.CurveBase]( \
                new _curves.CurveBase(
                    deref(crv_calendar._thisptr),
                    fixingDays,
                    deref(depositDayCounter._thisptr),
                    <_Frequency>fixedRateFrequency,
                    <_BusinessDayConvention>fixedInstrumentConvention,
                    deref(fixedInstrumentDayCounter._thisptr),
                    deref(termStructureDayCounter._thisptr)
                    )
                )

                      
cdef class RateHelperCurve:
    """Rate Helper Curve 
    
    """
    def __cinit__(self):
        self._thisptr = NULL

    def __dealloc__(self):
        if self._thisptr is not NULL:
            del self._thisptr
            
    def __init__(self, CurveBase curvebase):
        
        cdef _curves.CurveBase *_crvbase
        
        try:
            _crvbase = curvebase._thisptr.get()
            self._thisptr = new shared_ptr[_curves.RateHelperCurve]( \
                new _curves.RateHelperCurve(deref(_crvbase))
                )
        except:
            self._thisptr = new shared_ptr[_curves.RateHelperCurve]( \
                new _curves.RateHelperCurve()
                )

    def add_helpers(self, rh_type, curvemap):
        cdef _curves.CurveMap   _curvemap
        
        _curvemap = curveMap_from_dict(curvemap)
        
        if rh_type == DEPO:
            self._thisptr.get().add_depos(_curvemap)
        elif rh_type == FUT:
            self._thisptr.get().add_futs(_curvemap)
        elif rh_type == SWAP: 
            self._thisptr.get().add_swaps(_curvemap)
        else:
            raise ValueError, "Type: %s invalid ratehelper" % rh_type
            
    def update(self, depos=None, futures=None, swaps=None, evaldate=None):

        MSG_ARGS = "RateHelperCurve.update must have at least one curve"
        assert any((depos, futures, swaps)), MSG_ARGS
        
        cdef _curves.CurveMap   depocurve
        cdef _curves.CurveMap   futcurve
        cdef _curves.CurveMap   swapcurve
        
        #TODO: validate curve inputs
        #      check that tenors/future dates don't overlap
        if depos:
            depocurve = curveMap_from_dict(depos)

        if futures:
            futcurve = curveMap_from_dict(futures)
                
        if swaps:
            swapcurve = curveMap_from_dict(swaps)

        if evaldate:
            self.curveDate = evaldate
        
        print("\nin update: {}\n".format(get_eval_date()))
        self._thisptr.get().update(depocurve, futcurve, swapcurve) 
        print("in update: {}\n".format(get_eval_date()))
        
        return self.curveDate

    def validateNewCurve(self, depos=None, futures=None, swaps=None):
        new_crv = {}
        new_crv.update(swaps)
        new_crv.update(futures)
        new_crv.update(depos)
        
        prv_crv = self.curveQuotes
        if prv_crv and all([new_crv.get(h, None) for h in prv_crv]):
            isValid = True
        
        else:
            isValid = False
        
        return isValid

    def advanceCurveDate(self, int ndays, timeunit=None):
        
        if not timeunit:
            timeunit = TimeUnits.Days
            
        cdef int _ndays = int(ndays)
        
        self._thisptr.get().advanceCurveDate(int(ndays), 
                                             <_qlperiod.TimeUnit> timeunit)
        
    property curveDate:
        def __get__(self):
            cdef _qldate.Date _refdate = self._thisptr.get().curveDate()
            return _pydate_from_qldate(_refdate)
        
        def __set__(self, curve_date):
            cdef _qldate.Date _refdate
            
            try:
                _refdate = _qldate_from_pydate(curve_date)
            except:
                _refdate = _qldate_from_pydate(date.today())
            
            self._thisptr.get().setCurveDate(_refdate)
                
    property referenceDate:
        def __get__(self):
            cdef _qldate.Date _refdate = self._thisptr.get().referenceDate()
            return _pydate_from_qldate(_refdate)
    
    property maxDate:
        def __get__(self):
            cdef _qldate.Date _qdate_ref = self._thisptr.get().maxDate()
            return _pydate_from_qldate(_qdate_ref)
    
    property fixingdays:
        def __get__(self):
            cdef int fixdays_ref = self._thisptr.get().fixingDays()
            return fixdays_ref
    
    property curveQuotes:
        def __get__(self):
            cdef _curves.CurveMap crv = self._thisptr.get().curveQuotes()
            
            pycrv = dict_from_CurveMap(crv)
            return pycrv
    
    property calendar:
        def __get__(self):
            cdef  _calendar.Calendar crv_cal = self._thisptr.get().calendar()
            
            cdef string cal_name = crv_cal.name()
            
            return Calendars().get(cal_name.c_str(), None)
            
    # Curve functions
    def tenorquote(self, key):
        key = key.upper()
        cdef char* tnr = key
        cdef Rate rate = self._thisptr.get().tenorquote(<string>tnr)
        return rate
        
    def discount(self, ref):
        cdef double         yrs 
        cdef _qldate.Date   _date_ref
        cdef double discfactor
        
        if type(ref) == type(date.today()):
            _date_ref = _qldate_from_pydate(ref)
            discfactor = self._thisptr.get().discount(_date_ref, True)
            
        else:
            try:
                yrs = <double>ref
            except:
                yrs = 0.0
            
            discfactor = self._thisptr.get().discount(yrs, True)
        
        return discfactor
        


        
