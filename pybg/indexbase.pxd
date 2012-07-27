# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

from pybg.quantlib.handle cimport shared_ptr

from libcpp cimport bool
from libcpp.string cimport string

cimport pybg._indexbase as _indexbase
cimport pybg.quantlib.indexes._libor as _libor

cdef class USDLiborIndex:
    cdef shared_ptr[_indexbase.IndexBase[_libor.USDLibor]]* _thisptr

