# distutils: language = c++ 
# 

cimport pybg.version

cimport pybg.quantlib.time._date as _qldate
cimport pybg.quantlib.time.date as qldate

from libcpp.string cimport string

cpdef qldate.Date ql_date(object pydate)