"""
 Copyright (C) 2011, Enthought Inc
 Copyright (C) 2011, Patrick Henaff

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
"""

include '../types.pxi'
from cython.operator cimport dereference as deref

from pybg.quantlib.index cimport Index
from pybg.quantlib.handle cimport shared_ptr
from pybg.quantlib.time.date cimport Period
from pybg.quantlib.time.daycounter cimport DayCounter
from pybg.quantlib.currency cimport Currency
from pybg.quantlib.time.calendar cimport Calendar

cimport pybg.quantlib._index as _in
cimport pybg.quantlib.indexes._interest_rate_index as _iri

cdef extern from "string" namespace "std":
    cdef cppclass string:
        char* c_str()

cdef class InterestRateIndex(Index):
    def __cinit__(self):
        pass
        
    def __str__(self):
        return 'Interest rate index %s' % self.name
    
#    property tenor:
#        def __get__(self):
#            return self._thisptr.get().tenor()
            
#    property fixingDays:
#        def __get__(self):
#            return self._thisptr.fixingDays()
            
