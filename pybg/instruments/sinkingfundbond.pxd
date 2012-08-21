# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

include '../quantlib/types.pxi'
cimport pybg.version

from libcpp.vector cimport vector
from pybg.quantlib.handle cimport shared_ptr

from pybg.instruments.instrumentbases cimport BondBase
cimport pybg.instruments._instrumentbases as _instrumentbases

cimport pybg.instruments._sinkingfundbond as _sinkingfundbond

cdef class SinkingFundBond(BondBase):
    '''Sinking Fund Bond Structure
    
    '''
    pass