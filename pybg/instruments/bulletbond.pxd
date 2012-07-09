# distutils: language = c++
# distutils: libraries = QuantLib

from pybg.quantlib.handle cimport shared_ptr

cimport pybg.instruments._bulletbond as _bulletbond

cdef class BulletBond:
    '''Bullet Bond Structure
    
    '''
    cdef shared_ptr[_bulletbond.BulletBond]* _thisptr
