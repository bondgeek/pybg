# distutils: language = c++
# distutils: libraries = QuantLib

"""
 Copyright (C) 2011, Enthought Inc
 Copyright (C) 2011, Patrick Henaff

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
"""

include '../types.pxi'
from libcpp cimport bool
from pybg.quantlib.handle cimport Handle

from pybg.quantlib.time._date cimport Date
from pybg.quantlib.time._period cimport Period
from pybg.quantlib.time._calendar cimport Calendar, BusinessDayConvention
from pybg.quantlib.time._daycounter cimport DayCounter
from pybg.quantlib._currency cimport Currency

cimport pybg.quantlib.termstructures.yields._flat_forward as _ff
from pybg.quantlib.indexes._interest_rate_index cimport InterestRateIndex

cdef extern from "string" namespace "std":
    cdef cppclass string:
        char* c_str()    

cdef extern from 'ql/indexes/iborindex.hpp' namespace 'QuantLib':

    # base class for Inter-Bank-Offered-Rate indexes (e.g. %Libor, etc.)
    cdef cppclass IborIndex(InterestRateIndex):
        IborIndex()
        IborIndex(string& familyName,
                  Period& tenor,
                  Natural settlementDays,
                  Currency& currency,
                  Calendar& fixingCalendar,
                  BusinessDayConvention convention,
                  bool endOfMonth,
                  DayCounter& dayCounter) except +
                  # Handle[_ff.YieldTermStructure]& h) except +

        # \name Inspectors
        BusinessDayConvention businessDayConvention()
        bool endOfMonth()
        
        # the curve used to forecast fixings
        Handle[_ff.YieldTermStructure] forwardingTermStructure()
        
        # \name Date calculations
        Date maturityDate(Date& valueDate)
        
        # \name Other methods
        # returns a copy of itself linked to a different forwarding curve
        #virtual boost::shared_ptr<IborIndex> clone(
        #                const Handle<YieldTermStructure>& forwarding) const;

    cdef cppclass OvernightIndex(IborIndex):
        OvernightIndex()
        OvernightIndex(string& familyName,
                       Natural settlementDays,
                       Currency& currency,
                       Calendar& fixingCalendar,
                       DayCounter& dayCounter,
                       Handle[_ff.YieldTermStructure]& h)

        # returns a copy of itself linked to a different forwarding curve
        #boost::shared_ptr<IborIndex> clone(
        #                           const Handle<YieldTermStructure>& h) const;
