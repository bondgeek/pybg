# distutils: language = c++
# distutils: libraries = QuantLib
'''
        IndexBase()
        IndexBase(Integer n, QLTimeUnit units)
        IndexBase(string tenor)
        
'''
include 'quantlib/types.pxi'

from pybg.quantlib.handle cimport shared_ptr
from cython.operator cimport dereference as deref

from libcpp cimport bool
from libcpp.string cimport string

from pybg.quantlib.time._period cimport TimeUnit as QLTimeUnit

cimport _indexbase

cdef class USDLiborIndex:
    def __cinit__(self):
        self._thisptr = NULL

    def __dealloc__(self):
        if self._thisptr is not NULL:
            del self._thisptr
    
    def __init__(self, *args):
        
        cdef char* tnr
        cdef Integer n 
        cdef QLTimeUnit units
            
        if not args:
            self._thisptr = new shared_ptr[_indexbase.IndexBase[_libor.USDLibor]]( \
                new _indexbase.IndexBase[_libor.USDLibor]()
            )
        
        elif len(args)==1:
            tnr = args[0]
            
            self._thisptr = new shared_ptr[_indexbase.IndexBase[_libor.USDLibor]]( \
                new _indexbase.IndexBase[_libor.USDLibor]( <string>tnr )
            )
        
        elif len(args)==2:
            self._thisptr = new shared_ptr[_indexbase.IndexBase[_libor.USDLibor]]( \
                new _indexbase.IndexBase[_libor.USDLibor]( <Integer>args[0], <QLTimeUnit>args[2] )
            )
    


    
