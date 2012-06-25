cimport _rate_helpers as _rh
from pybg.quantlib.handle cimport shared_ptr

cdef class RateHelper:
    cdef shared_ptr[_rh.RateHelper]* _thisptr

cdef class RelativeDateRateHelper:
    cdef shared_ptr[_rh.RelativeDateRateHelper]* _thisptr
    
# DepositRateHelper
# FraRateHelper
# FuturesRateHelper
# SwapRateHelper
