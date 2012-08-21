# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like
include '../quantlib/types.pxi'
cimport pybg.version

from pybg.quantlib.handle cimport shared_ptr

from pybg.instruments.instrumentbases cimport BondBase
cimport pybg.instruments._instrumentbases as _instrumentbases

cimport pybg.instruments._bulletbond as _bulletbond

cdef class BulletBond(BondBase):
    '''Bullet Bond Structure
    
    '''
    #cdef shared_ptr[_bulletbond.BulletBond]* _thisptr
    cdef int _settlementDays