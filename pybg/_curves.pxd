# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

include 'quantlib/types.pxi'
cimport pybg.version

from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.map cimport map
from libcpp.string cimport string

from pybg.quantlib.handle cimport shared_ptr
from pybg.quantlib.time._date cimport Date as _QLDate
from pybg.quantlib.time._period cimport Frequency 
from pybg.quantlib.time._calendar cimport Calendar
from pybg.quantlib.time._period cimport TimeUnit
from pybg.quantlib.time._calendar cimport BusinessDayConvention
from pybg.quantlib.time._daycounter cimport DayCounter as DayCounter

from cython.operator cimport dereference as deref

cdef extern from 'bg/curvebase.hpp' namespace 'bondgeek':
    cdef enum RHType: 
        DEPO
        FRA
        FUT
        SWAP


cdef extern from 'bg/curvebase.hpp' namespace 'bondgeek':
        
    cdef cppclass CurveBase:
        CurveBase() except +        
        CurveBase(Calendar              calendar,
                  Integer               fixingDays,                  
                  DayCounter            depositDayCounter,
                  Frequency             fixedRateFrequency,
                  BusinessDayConvention fixedInstrumentConvention, 
                  DayCounter            fixedInstrumentDayCounter, 
                  DayCounter            termStructureDayCounter
                  )
                      
        # Inspectors
        _QLDate     curveDate() 
        _QLDate     referenceDate()
        _QLDate     maxDate()
        Calendar    calendar()
        Calendar    fixingCalendar()        
        Integer     fixingDays() 

        # build methods
        _QLDate     setCurveDate(_QLDate todays_date)
        _QLDate     advanceCurveDate(int n, TimeUnit unit)
        void        build()
        
        # Results
        Real        discount(double years, bool extrapolate) except +
        

cdef extern from 'bg/curves/ratehelpercurve.hpp' namespace 'bondgeek':
    
    ctypedef map[string, Rate] CurveMap
    
    cdef cppclass RateHelperCurve(CurveBase):
        RateHelperCurve() except +
        RateHelperCurve(CurveBase crvtype) except +
        
        void add_depos(CurveMap crv)
        void add_swaps(CurveMap crv)
        void add_futs(CurveMap crv)
        
        void update(CurveMap &depocurve,
                    CurveMap &futcurve,
                    CurveMap &swapcurve
                    )
        void update(CurveMap &depocurve,
                    CurveMap &futcurve,
                    CurveMap &swapcurve,
                    _QLDate todaysDate
                    )
                    
        # curve values        
        Real        tenorquote(string key) except +
        CurveMap    curveQuotes() except +

        
cdef extern from 'bg/curves/bondcurve.hpp' namespace 'bondgeek':
    
    cdef cppclass BondHelperQuote:
        BondHelperQuote()
        BondHelperQuote(Real px_quote, 
                        _QLDate maturity, 
                        Rate coupon)
        BondHelperQuote(Real px_quote, 
                        _QLDate maturity, 
                        Rate coupon,
                        _QLDate issue_date) 
                        
        #Inspectors
        _QLDate issue_date()
        _QLDate maturity() 
        Real coupon()
        Real quote() 
    
    ctypedef map[string, BondHelperQuote] BondCurveMap
    
    cdef cppclass BondCurve(RateHelperCurve, CurveBase):
        BondCurve() except +
        BondCurve(CurveBase crvtype) except +
        
        void add_bonds(BondCurveMap crv) 

        void update(BondCurveMap bondcurve,
                    CurveMap     depocurve
                    )
        void update(BondCurveMap bondcurve,
                    CurveMap     depocurve,
                    _QLDate      todays_date
                    )
        
        bool updateBondCurve(BondCurveMap crv) except +
