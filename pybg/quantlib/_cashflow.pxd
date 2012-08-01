include 'types.pxi'
cimport pybg.version

from libcpp cimport bool
from libcpp.vector cimport vector

from pybg.quantlib.handle cimport shared_ptr
from pybg.quantlib.time._date cimport Date

cdef extern from 'boost/optional.hpp' namespace 'boost':
    cdef cppclass optional[T]:
        optional(T*)

cdef extern from 'ql/event.hpp' namespace 'QuantLib':
    cdef cppclass Event:
        
        Date date()
        bool hasOccurred (Date& refDate,
                          optional[bool] includeRefDate)

cdef extern from 'ql/cashflow.hpp' namespace 'QuantLib':
    cdef cppclass CashFlow(Event):
        
        Real amount()
    
    ctypedef vector[shared_ptr[CashFlow]] Leg

cdef extern from 'ql/cashflows/simplecashflow.hpp' namespace 'QuantLib':
    cdef cppclass SimpleCashFlow(CashFlow):
        SimpleCashFlow(Real amount,
                       Date& date)
        

