# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

include '../quantlib/types.pxi'
cimport pybg.version

from libcpp.vector cimport vector
from pybg.quantlib.handle cimport shared_ptr

cimport pybg.instruments._sinkingfundbond as _sinkingfundbond

cdef class SinkingFundBond:
    '''Sinking Fund Bond Structure
    
    '''
    cdef shared_ptr[_sinkingfundbond.SinkingFundBond]* _thisptr
    cdef int _settlementDays
    cdef Real coupon