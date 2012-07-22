# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

include 'types.pxi'

from libcpp cimport bool

cdef extern from 'ql/quote.hpp' namespace 'QuantLib':
    cdef cppclass Quote:
        Quote() except +
        Real value() except +
        bool isValid() except +

cdef extern from 'ql/quotes/simplequote.hpp' namespace 'QuantLib':

    cdef cppclass SimpleQuote(Quote):
        SimpleQuote(Real value) except +
        Real setValue(Real value) except +
