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
cimport pybg.version

from pybg.quantlib.handle cimport Handle

cimport pybg.quantlib.termstructures.yields._flat_forward as _ff

from pybg.quantlib._currency cimport Currency
from pybg.quantlib.indexes._ibor_index cimport IborIndex
from pybg.quantlib.time._calendar cimport Calendar
from pybg.quantlib.time._daycounter cimport DayCounter
from pybg.quantlib.time._period cimport Period

cdef extern from "string" namespace "std":
    cdef cppclass string:
        char* c_str()

cdef extern from 'ql/indexes/ibor/libor.hpp' namespace 'QuantLib':

    cdef cppclass Libor(IborIndex):
        Libor()
        Libor(string& familyName,
                  Period& tenor,
                  Natural settlementDays,
                  Currency& currency,
                  Calendar& finencialCenterCalendar,
                  DayCounter& dayCounter) except +

cdef extern from 'ql/indexes/ibor/usdlibor.hpp' namespace 'QuantLib':

    cdef cppclass USDLibor(Libor):
        USDLibor()
        USDLibor(Period& tenor,
                 Handle[_ff.YieldTermStructure] h
                 ) except +