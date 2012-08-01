# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

"""
 Copyright (C) 2011, Enthought Inc
 Copyright (C) 2011, Patrick Henaff

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
"""

include '../../types.pxi'
cimport pybg.version

from libcpp cimport bool
from libcpp.vector cimport vector

from pybg.quantlib.handle cimport shared_ptr, Handle, RelinkableHandle
from pybg.quantlib.time._calendar cimport Calendar
from pybg.quantlib.time._date cimport Date
from pybg.quantlib.time._daycounter cimport DayCounter
from pybg.quantlib.time._period cimport Frequency
cimport pybg.quantlib._quote as _qt

cdef extern from 'ql/compounding.hpp' namespace 'QuantLib':
    cdef enum Compounding:
        Simple = 0
        Compounded = 1
        Continuous = 2
        SimpleThenCompounded = 3

cdef extern from 'ql/termstructures/yieldtermstructure.hpp' namespace 'QuantLib':

    cdef cppclass YieldTermStructure:

        YieldTermStructure() except +
        YieldTermStructure(DayCounter& dc,
                           vector[Handle[_qt.Quote]]& jumps,
                           vector[Date]& jumpDates,
                           ) except +
        DiscountFactor discount(Date& d) except +
        DiscountFactor discount(Date& d, bool extrapolate) except +
        DiscountFactor discount(Time t) except +
        DiscountFactor discount(Time t, bool extrapolate) except +
        Date& referenceDate()

cdef extern from 'ql/termstructures/yield/flatforward.hpp' namespace 'QuantLib':

    cdef cppclass FlatForward(YieldTermStructure):

        FlatForward(DayCounter& dc,
                   vector[Handle[_qt.Quote]]& jumps,
                   vector[Date]& jumpDates,
        ) except +


        FlatForward(Date& referenceDate,
                   Rate forward,
                   DayCounter& dayCounter,
                   Compounding compounding,
                   Frequency frequency
        ) except +

        FlatForward(Natural settlementDays,
                    Calendar& calendar,
                    Rate forward,
                    DayCounter& dayCounter,
                    Compounding compounding,
                    Frequency frequency
        ) except +

        # from days and quote :
        FlatForward(Natural settlementDays,
                    Calendar& calendar,
                    Handle[_qt.Quote]& forward,
                    DayCounter& dayCounter,
        ) except +
        
        FlatForward(Natural settlementDays,
                    Calendar& calendar,
                    Handle[_qt.Quote]& forward,
                    DayCounter& dayCounter,
                    Compounding compounding,
        ) except +
        
        FlatForward(Natural settlementDays,
                    Calendar& calendar,
                    Handle[_qt.Quote]& forward,
                    DayCounter& dayCounter,
                    Compounding compounding,
                    Frequency frequency
        ) except +
        
        # from date and forward
        FlatForward(Date& referenceDate,
                    Handle[_qt.Quote]& forward,
                    DayCounter& dayCounter,
                    Compounding compounding,
                    Frequency frequency
        ) except +
