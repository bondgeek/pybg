# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like
include '../quantlib/types.pxi'

from pybg.quantlib.handle cimport shared_ptr

from pybg.instruments.instrumentbases cimport BondBase
cimport pybg.instruments._instrumentbases as _instrumentbases

cimport pybg.instruments._callbond as _callbond

cdef class CallBond(BondBase):
    '''
    Callable Bond Structure for OAS calcs
    
    Also includes after tax yield calculations for bonds subject to 
    accrued market discount
    
    '''
    pass