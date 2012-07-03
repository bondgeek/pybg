# distutils: language = c++
# distutils: libraries = QuantLib

import datetime  

from cython.operator cimport dereference as deref

from libcpp cimport bool as bool
from libcpp.string cimport string

cdef extern from "bg/date_utilities.hpp" namespace "bondgeek":
    _qldate.Date get_evaluation_date()
    void set_evaluation_date(_qldate.Date& date)
    void set_evaluation_date(int& cob_date)
    void set_evaluation_date(string& cob_date)
    
    #Date interfaces
    _qldate.Date QDate(_qldate.Date& indate)
    _qldate.Date QDate(string indate)
    _qldate.Date QDate(int indate)

    # IMM Interfaces
    bool            isIMMdate(_qldate.Date date_ref, bool mainCycle)
    bool            isIMMcode(string instring, bool mainCycle)
    string          imm_code(_qldate.Date date_ref)
    _qldate.Date    imm_date(string immCode, _qldate.Date ref_date)
    _qldate.Date    imm_nextDate(_qldate.Date date_ref, bool mainCycle)
    string          imm_nextCode(string immCode, 
                                 _qldate.Date date_ref,
                                 bool mainCycle)


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

cdef _qldate.Date _qldate_from_pydate(object pydate):
    cdef qldate.Date qdate_ref = qldate.Date.from_datetime(pydate)
    cdef _qldate.Date* date_ref = <_qldate.Date*>qdate_ref._thisptr.get()
    
    return deref(date_ref)
    

cpdef qldate.Date qldate_from_pydate(object pydate):
    cdef qldate.Date qdate_ref = qldate.Date.from_datetime(pydate)
    return qdate_ref
    
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
            
            cdef _qldate.Date date_ref = _qldate_from_pydate(evaluation_date)
            set_evaluation_date(date_ref)
            
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
        
# IMM Inteface
cdef class IMM:
    @classmethod
    def nextDate(cls, date_ref=None):
        
        if not date_ref:
            date_ref = get_eval_date()
            
        cdef _qldate.Date qdate = _qldate_from_pydate(date_ref)
        qdate = imm_nextDate(qdate, 1)
            
        return _pydate_from_qldate(qdate)
    
    @classmethod
    def nextCode(cls, tickercode=None, referenceDate=None):
        
        cdef _qldate.Date qdate
        cdef string qticker
        cdef char* cticker
        
        if not referenceDate:
            referenceDate = get_eval_date()
        
        if not tickercode:
            qdate = _qldate_from_pydate(referenceDate)
            qdate = imm_nextDate(qdate, 1)
            qticker = imm_code(qdate)
        
        else:
            qdate   = _qldate_from_pydate(referenceDate)
            cticker = tickercode
            qticker = imm_nextCode(<string>cticker,
                                   qdate,
                                   1)
            
        return qticker.c_str()

    @classmethod 
    def tenor(cls, ticker):
        try:
            n = int( ticker.upper().strip().strip('ED') )
        
        except:
            n = 1
        
        return n
            
    @classmethod
    def contract_date(cls, ticker='ED1', referenceDate=None):
        '''Return contract date from generic ticker.
        
        E.g., IMM.contract_date('ED2') yields ticker for second contract
        
        '''
        n = cls.tenor(ticker)
            
        tkr_list = [dt for dt in cls.futures_strip_dates(n)]
        
        return tkr_list[n-1] if tkr_list else None
        


    @classmethod
    def futures_strip_dates(cls, n=40, referenceDate=None):
        '''Iterator for generating strip of eurodollar futures dates
        
        '''
        limit = n
        countr = 0

        while countr < limit:
            referenceDate = cls.nextDate(referenceDate)
            yield referenceDate
            countr += 1


    @classmethod
    def futures_strip(cls, n=40, code=None):
        '''Iterator for generating strip of eurodollar futures tickers
        '''
        limit = n
        countr = 0

        while countr < limit:
            code = cls.nextCode(code)
            yield code
            countr += 1

