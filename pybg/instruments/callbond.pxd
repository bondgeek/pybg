# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like
include '../quantlib/types.pxi'

from pybg.quantlib.handle cimport shared_ptr

cimport pybg.instruments._callbond as _callbond

cdef class CallBond:
    '''Callable Bond Structure for OAS calcs
    
    '''
    cdef shared_ptr[_callbond.CallBond]* _thisptr
    cdef Real _coupon
    cdef int _settlementDays