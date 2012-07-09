# distutils: language = c++
# distutils: libraries = QuantLib

from pybg.quantlib.handle cimport Handle
cimport pybg.quantlib.termstructures.yields._flat_forward as _ff

from pybg.quantlib.indexes._ibor_index cimport IborIndex

from pybg.quantlib.time._period cimport Period

cdef extern from 'ql/indexes/ibor/euribor.hpp' namespace 'QuantLib':

    cdef cppclass Euribor(IborIndex):
        Euribor()
        Euribor(Period& tenor,
                Handle[_ff.YieldTermStructure] h
                ) except +    
                 
    cdef cppclass Euribor6M(Euribor):
        Euribor6M(Handle[_ff.YieldTermStructure]& yc)
