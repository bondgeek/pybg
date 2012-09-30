# distutils: language = c++ 
# 

cimport pybg.version

cimport pybg.quantlib.time._date as _qldate
cimport pybg.quantlib.time.date as qldate

cdef class Settings:
    cdef object calendar
    cdef object convention