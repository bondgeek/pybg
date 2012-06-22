# distutils: language = c++
# distutils: libraries = QuantLib


cimport pybg.quantlib.time._date as _qldate
cimport pybg.quantlib.time.date as qldate

cpdef object pydate_from_qldate(qldate.Date qdate)