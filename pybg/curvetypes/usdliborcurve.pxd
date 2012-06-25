# distutils: language = c++
# distutils: libraries = QuantLib

from pybg.curves cimport CurveBase

# Specific Curve types

cdef class USDLiborCurve(CurveBase):
    pass