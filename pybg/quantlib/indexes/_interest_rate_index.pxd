# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

"""
 Copyright (C) 2011, Enthought Inc
 Copyright (C) 2011, Patrick Henaff

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
"""

include '../types.pxi'
from libcpp cimport bool

from pybg.quantlib._index cimport Index
from pybg.quantlib.time._date cimport Date
from pybg.quantlib.time._period cimport Period
from pybg.quantlib.time._calendar cimport Calendar
from pybg.quantlib.time._daycounter cimport DayCounter
from pybg.quantlib._currency cimport Currency

cdef extern from "string" namespace "std":
    cdef cppclass string:
        char* c_str()    

cdef extern from 'ql/indexes/interestrateindex.hpp' namespace 'QuantLib':

    cdef cppclass InterestRateIndex(Index):
        InterestRateIndex()
#        InterestRateIndex(string& familyName,
#                          Period& tenor,
#                          Natural settlementDays,
#                          Currency& currency,
#                          Calendar& fixingCalendar,
#                          DayCounter& dayCounter)
        string name()
        Calendar fixingCalendar( )
        bool isValidFixingDate(Date& fixingDate)
        Rate fixing(Date& fixingDate,
                    bool forecastTodaysFixing)
        update()
        string familyName()
        Period tenor()
        Natural fixingDays()
        Date fixingDate(Date& valueDate)
        Currency& currency()
        DayCounter& dayCounter()

        Date valueDate(Date& fixingDate)
        Date maturityDate(Date& valueDate)
