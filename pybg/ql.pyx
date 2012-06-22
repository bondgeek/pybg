# distutils: language = c++
# distutils: libraries = QuantLib

from cython.operator cimport dereference as deref

from libcpp cimport bool as cbool
from libcpp.string cimport string

cimport pybg.quantlib.time._date as __qldate
cimport pybg.quantlib.time.date as qldate

import datetime  

cdef extern from "bg/date_utilities.hpp" namespace "bondgeek":
    _qldate.Date get_evaluation_date()
    void set_evaluation_date(_qldate.Date& date)
    void set_evaluation_date(int& cob_date)
    void set_evaluation_date(string& cob_date)
    
    #Date interfaces
    _qldate.Date QDate(_qldate.Date& indate)
    _qldate.Date QDate(string indate)
    _qldate.Date QDate(int indate)

cdef extern from 'ql/version.hpp':

    char* QL_VERSION
    int QL_HEX_VERSION
    char* QL_LIB_VERSION

__quantlib_version__ = QL_VERSION
__quantlib_lib_version__ = QL_LIB_VERSION
__quantlib_hex_version__ = QL_HEX_VERSION


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
    
# Global Settings
#    Using datetime objects for interface with the real world
cdef class Settings:

    def __init__(self):
        pass

    property evaluation_date:
        """Property to set/get the evaluation date. 
        
        """
        def __get__(self):
            cdef _qldate.Date evaluation_date = get_evaluation_date()
            return _pydate_from_qldate(evaluation_date)

        def __set__(self, evaluation_date):
            # TODO: logic to pick what kind of date is input
            #       i.e., datetime.date, ccyymmdd int, excel int, string...
            #       or just leave it the user to transform to a datetime object
            
            #cdef _qldate.Date* date_ref = <_qldate.Date*>evaluation_date._thisptr.get()
            cdef qldate.Date qdate_ref = qldate.Date.from_datetime(evaluation_date)
            cdef _qldate.Date* date_ref = <_qldate.Date*>qdate_ref._thisptr.get()
            set_evaluation_date(deref(date_ref))

    property version:
        """Returns the QuantLib C++ version (QL_VERSION) used by this wrapper.
        
        """
        def __get__(self):
            return QL_VERSION

    @classmethod
    def instance(cls):
        """ Returns an instance of the global Settings object.

        Utility method to mimic the behaviour of the C++ singleton.

        """

        return cls()

def get_eval_date():
    return Settings().evaluation_date
    
def set_eval_date(eval_date=None):
    
    dt = Settings().instance().evaluation_date
    if not eval_date:
        eval_date = datetime.date.today()
        
    try:
        Settings().evaluation_date = eval_date
    except:
        print("\nError setting evaluation date: %s" % eval_date)
        Settings().instance().evaluation_date = dt
        print("Defaulting to %s\n" % dt)
        