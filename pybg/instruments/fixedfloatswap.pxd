# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

from pybg.quantlib.handle cimport shared_ptr

cimport pybg._curves as _curves
cimport pybg._indexbase as _indexbase
cimport pybg.quantlib.indexes._libor as _libor
cimport pybg.quantlib.indexes._euribor as _euribor

cimport pybg.instruments._fixedfloatswap as _fixedfloatswap

from pybg.quantlib.indexes._ibor_index cimport IborIndex

cdef class LiborSwap:
    '''Libor Swap factory
    
    Uses c++ SwapType
    '''
    
    cdef shared_ptr[_fixedfloatswap.FixedFloatSwap]* _thisptr

cdef class USDLiborSwap(LiborSwap):
    '''USD Libor Swap
    
    '''
    cdef _fixedfloatswap.SwapType[ _libor.USDLibor]* _swaptype
    
cdef class EuriborSwap(LiborSwap):
    '''USD Libor Swap
    
    '''
    cdef _fixedfloatswap.SwapType[ _euribor.Euribor]* _swaptype