# distutils: language = c++
# distutils: libraries = QuantLib

cimport pybg.quantlib.time._date as _qldate
cimport pybg.quantlib.time.date as qldate

cdef object _pydate_from_qldate(_qldate.Date qdate)

cpdef object pydate_from_qldate(qldate.Date qdate)

cdef _qldate.Date _qldate_from_pydate(object pydate)

cpdef qldate.Date qldate_from_pydate(object pydate)