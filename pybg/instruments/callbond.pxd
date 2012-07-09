# distutils: language = c++
# distutils: libraries = QuantLib

from pybg.quantlib.handle cimport shared_ptr

cimport pybg.instruments._callbond as _callbond

cdef class CallBond:
    '''Callable Bond Structure for OAS calcs
    
    '''
    cdef shared_ptr[_callbond.CallBond]* _thisptr
