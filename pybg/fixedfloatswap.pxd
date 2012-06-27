# distutils: language = c++
# distutils: libraries = QuantLib

from pybg.quantlib.handle cimport shared_ptr

cimport _indexbase
cimport _fixedfloatswap

cimport pybg.quantlib.indexes._libor as _libor

cdef class USDLiborSwap:
    '''USD Libor Swap factory
    
    Uses c++ SwapType
    '''
    cdef shared_ptr[_fixedfloatswap.FixedFloatSwap]* _thisptr
