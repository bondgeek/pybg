# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

include '../quantlib/types.pxi'
cimport pybg.version

from libcpp cimport bool

from pybg.quantlib.time._date cimport Date as _QLDate
from pybg.quantlib.time._period cimport Frequency 
from pybg.quantlib.time._calendar cimport Calendar
from pybg.quantlib.time._calendar cimport BusinessDayConvention
from pybg.quantlib.time._daycounter cimport DayCounter as DayCounter

from pybg.quantlib._cashflow cimport Leg

cimport pybg._curves as _curves
cimport pybg.instruments._bulletbond as _bulletbond
from pybg.instruments._instrumentbases cimport BondBase

cdef extern from 'bg/instruments/callbond.hpp' namespace 'bondgeek':

    cdef cppclass CallBond(BondBase):
        CallBond(Rate &coupon,
                 _QLDate &maturity,
                 _QLDate &callDate,
                 Real &callPrice, 
                 _QLDate &issue_date,
                 Calendar calendar, # = UnitedStates(UnitedStates::GovernmentBond)
                 Natural settlementDays, # = 3
                 DayCounter daycounter, # = ActualActual(ActualActual::Bond)
                 Frequency payFrequency, # = Semiannual
                 Frequency callFrequency, # = Semiannual
                 Real redemption, # = 100.0
                 Real faceamount, # = 100.0
                 BusinessDayConvention accrualConvention, # = Unadjusted
                 BusinessDayConvention paymentConvention, # = Unadjusted
                 _QLDate eval_date
                 )
                 
        CallBond(Rate &coupon,
                 _QLDate &maturity,
                 _QLDate &issue_date,
                 Calendar calendar, # = UnitedStates(UnitedStates::GovernmentBond)
                 Natural settlementDays, # = 3
                 DayCounter daycounter, # = ActualActual(ActualActual::Bond)
                 Frequency payFrequency, # = Semiannual
                 Frequency callFrequency, # = Semiannual
                 Real redemption, # = 100.0
                 Real faceamount, # = 100.0
                 BusinessDayConvention accrualConvention, # = Unadjusted
                 BusinessDayConvention paymentConvention, # = Unadjusted
                 _QLDate eval_date
                 )

        # Engines 
        void setEngine(_curves.CurveBase crv,
                       Real              a, 
                       Real              sigma,
                       bool              lognormal
                       ) 
                       
        # Inspectors
        Real reversionParameter()
        Real sigma()
        bool lognormal()
        
        _QLDate maturityDate()
        _QLDate settlementDate()
        _QLDate settlementDate(_QLDate)
        _QLDate issueDate()
        DayCounter dayCounter()
        Frequency frequency()
        Natural settlementDays()
        Calendar calendar()
        Leg redemptions()
        Leg cashflows()

        int ytwFeature()
        
        # Bond Math
        double toYTM()
        double toYTM(Real bondprice)
        double toYTM(Real bondprice, Real redemption)
        double ytmToPrice(Real bondyield)

        #// OAS Functions
        void oasEngine(_curves.CurveBase &crv, 
                       Real              &a, 
                       Real              &sigma,
                       Real              &spread,
                       bool              lognormal
                       ) 
                       
        void setSpread(Real &spread)
        Real getSpread()
        
        #// OAS Functions
        #// Functions to price bond off oas engine, given spread and vol
        Real oasValue(Real &spread, Real &sigma, Real a, bool lognormal)

        #// Solve for spread or implied volatility
        Real oas(Real &bondprice,
                 Real sigma, 
                 Real a,
                 bool lognormal
                 )
		
        Real oasImpliedVol(Real &bondprice, 
                           Real spread,
                           Real a,
                           bool lognormal
                           )
        