# distutils: language = c++
# distutils: libraries = QuantLib
'''
BulletBond(const Rate &coupon,
                   const Date &maturity,
                   const Date &issue_date = Date(),
				   Calendar calendar = UnitedStates(UnitedStates::GovernmentBond),
				   Natural settlementDays = 3,
				   DayCounter daycounter = ActualActual(ActualActual::Bond),
				   Frequency frequency = Semiannual,
				   Real redemption = 100.0,
				   Real faceamount = 100.0,
				   BusinessDayConvention accrualConvention = Unadjusted,
				   BusinessDayConvention paymentConvention = Unadjusted
                   );
        
        double toPrice();
        double toPrice(Rate &);
        
        double toYield();
        double toYield(Rate &);
		
		virtual void setEngine(CurveBase &crv) ;
'''

include '../quantlib/types.pxi'

from cython.operator cimport dereference as deref

from pybg.quantlib.handle cimport shared_ptr
from pybg.quantlib.time._date cimport Date as _QLDate
from pybg.quantlib.time._period cimport Frequency 
from pybg.quantlib.time._calendar cimport Calendar
from pybg.quantlib.time._calendar cimport BusinessDayConvention
from pybg.quantlib.time._daycounter cimport DayCounter as DayCounter

cimport pybg._curves as _curves

cdef extern from 'bg/instruments/bulletbond.hpp' namespace 'bondgeek':

    cdef cppclass BulletBond:
        BulletBond(
                       Rate coupon,
                       _QLDate maturity,
                       _QLDate issue_date,
                       Calendar calendar, # = UnitedStates(UnitedStates::GovernmentBond)
                       Natural settlementDays, # = 3
                       DayCounter daycounter, # = ActualActual(ActualActual::Bond)
                       Frequency frequency, # = Semiannual
                       Real redemption, # = 100.0
                       Real faceamount, # = 100.0
                       BusinessDayConvention accrualConvention, # = Unadjusted
                       BusinessDayConvention paymentConvention, # = Unadjusted
                       ) except +
                   
        void setEngine(_curves.CurveBase crv)
        
        double toPrice()
        double toPrice(Real bondyield)
        
        double toYield()
        double toYield(Rate bondprice)