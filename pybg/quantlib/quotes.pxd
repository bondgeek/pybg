# distutils: language = c++

cimport _quote as _qt

from pybg.quantlib.handle cimport shared_ptr

cdef class Quote:
    cdef shared_ptr[_qt.Quote]* _thisptr

cdef class SimpleQuote(Quote):
    pass
