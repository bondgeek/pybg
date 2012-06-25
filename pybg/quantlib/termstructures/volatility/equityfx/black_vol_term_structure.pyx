from cython.operator cimport dereference as deref
cimport _black_vol_term_structure as _bv


from pybg.quantlib.time.calendar cimport Calendar
from pybg.quantlib.time.date cimport Date
from pybg.quantlib.time.daycounter cimport DayCounter

cdef class BlackVolTermStructure:

    def __cinit__(self):
        self._thisptr = NULL

    def __dealloc__(self):
        if self._thisptr is not NULL:
            del self._thisptr


cdef class BlackConstantVol(BlackVolTermStructure):

    def __init__(self,
        Date reference_date,
        Calendar calendar,
        float volatility,
        DayCounter daycounter
    ):

        self._thisptr = new shared_ptr[_bv.BlackVolTermStructure](
            new _bv.BlackConstantVol(
                deref(reference_date._thisptr.get()),
                deref(calendar._thisptr),
                volatility,
                deref(daycounter._thisptr)
            )
        )

