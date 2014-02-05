# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

include 'quantlib/types.pxi'

cdef extern from 'ql/quantlib.hpp':
    
    char* QL_VERSION
    int QL_HEX_VERSION
    char* QL_LIB_VERSION
    
