# distutils: language = c++
# distutils: libraries = QuantLib

from pybg.quantlib.handle cimport shared_ptr

from libcpp cimport bool
from libcpp.string cimport string

cimport _indexbase
cimport pybg.quantlib.indexes._libor as _libor

cdef class USDLiborIndex:
    cdef shared_ptr[_indexbase.IndexBase[_libor.USDLibor]]* _thisptr

