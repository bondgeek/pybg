# distutils: language = c++
# distutils: libraries = QuantLib

include 'quantlib/types.pxi'

from pybg.quantlib.handle cimport shared_ptr

from cython.operator cimport dereference as deref

from libcpp cimport bool
from libcpp.string cimport string

from pybg.quantlib.time._period cimport (
    Period as QlPeriod, 
    TimeUnit as QLTimeUnit
    )
from pybg.quantlib.time._calendar cimport BusinessDayConvention
from pybg.quantlib.indexes._ibor_index cimport IborIndex as QLIborIndex

cimport pybg.quantlib.termstructures.yields._flat_forward as _ff

cdef extern from 'bg/indexbase.hpp' namespace 'bondgeek':
    cdef cppclass IndexBase[T]:
        IndexBase()
        IndexBase(Integer n, QLTimeUnit units)
        IndexBase(string tenor)

        BusinessDayConvention businessDayConvention()
        bool endOfMonth()
        
        shared_ptr[QLIborIndex] operator()()
        shared_ptr[QLIborIndex] operator()(shared_ptr[_ff.YieldTermStructure] ts)