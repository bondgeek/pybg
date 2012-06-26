# distutils: language = c++
# distutils: libraries = QuantLib

from pybg.quantlib.handle cimport shared_ptr

cimport _fixedfloatswap

cdef class FixedFloatSwap:
    cdef shared_ptr[_fixedfloatswap.FixedFloatSwap]* _thisptr