"""
 Copyright (C) 2011, Enthought Inc
 Copyright (C) 2011, Patrick Henaff

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
"""

include '../types.pxi'

from pybg.quantlib.handle cimport Handle, shared_ptr
from cython.operator cimport dereference as deref
from cpython cimport bool
from libcpp cimport bool as cbool

from libcpp.string cimport string
from cpython.string cimport PyString_AsString

cimport pybg.quantlib.termstructures.yields._flat_forward as _ffwd
cimport _libor
cimport pybg.quantlib._index as _in

from pybg.quantlib.termstructures.yields.flat_forward cimport YieldTermStructure
from pybg.quantlib.time.date cimport Period
from pybg.quantlib.time.daycounter cimport DayCounter
from pybg.quantlib.currency cimport Currency
from pybg.quantlib.time.calendar cimport Calendar
from pybg.quantlib.time._calendar cimport BusinessDayConvention


cdef class Libor(IborIndex):
    """ Base class for all BBA LIBOR indexes but the EUR, O/N, and S/N ones
    LIBOR fixed by BBA.

        See <http://www.bba.org.uk/bba/jsp/polopoly.jsp?d=225&a=1414>.
    """

    def __cinit__(self):
        self._thisptr = NULL

    def __dealloc__(self):
        if self._thisptr is not NULL:
            del self._thisptr
            self._thisptr = NULL

    def __init__(self,
        str familyName,
        Period tenor,
        Natural settlementDays,
        Currency currency,
        Calendar financial_center_calendar,
        DayCounter dayCounter):
    
        # convert the Python str to C++ string
        cdef string familyName_string = string(PyString_AsString(familyName))

        
        self._thisptr = new shared_ptr[_in.Index](
        new _libor.Libor(
            familyName_string,
            deref(tenor._thisptr.get()),
            <Natural> settlementDays,
            deref(currency._thisptr),
            deref(financial_center_calendar._thisptr),
            deref(dayCounter._thisptr)))

