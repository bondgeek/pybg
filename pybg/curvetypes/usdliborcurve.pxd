# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

from pybg.curves cimport CurveBase

# Specific Curve types

cdef class USDLiborCurve(CurveBase):
    pass