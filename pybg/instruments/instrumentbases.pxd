# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like
include '../quantlib/types.pxi'
cimport pybg.version

from pybg.quantlib.handle cimport shared_ptr

cimport pybg.instruments._instrumentbases as _instrumentbases

cdef class BondBase:
    cdef shared_ptr[_instrumentbases.BondBase]* _thisptr

    