'''
Core qldate to pydate utilities from bg c++ library, including IMM date handling
'''

import datetime  

from cython.operator cimport dereference as deref

from libcpp cimport bool as bool
from libcpp.string cimport string

from pybg.version import version

# Date Interfaces
cdef object _pydate_from_qldate(_qldate.Date qdate):
    """c++ QuantLib::Date to python datetime.time
    
    """
    cdef int m = qdate.month()
    cdef int d = qdate.dayOfMonth()
    cdef int y = qdate.year()
    
    return datetime.date(y, m, d)
    
cpdef object pydate_from_qldate(qldate.Date qdate):
    """cython Date to python datetime.time
    
    """
    cdef int m = qdate.month
    cdef int d = qdate.day
    cdef int y = qdate.year
    
    return datetime.date(y, m, d)

cdef _qldate.Date _qldate_from_pydate(object pydate):
    cdef qldate.Date qdate_ref = qldate.Date.from_datetime(pydate)
    cdef _qldate.Date* date_ref = <_qldate.Date*>qdate_ref._thisptr.get()
    
    return deref(date_ref)
    

cpdef qldate.Date qldate_from_pydate(object pydate):
    cdef qldate.Date qdate_ref = qldate.Date.from_datetime(pydate)
    return qdate_ref
    
                    
