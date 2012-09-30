# imm.pyx
#  

from libcpp cimport bool as bool
from libcpp.string cimport string

from pybg.ql import qldate_from_pydate, pydate_from_qldate
from pybg.ql cimport _pydate_from_qldate, _qldate_from_pydate


from pybg.settings import get_eval_date
    
from pybg.version import version

cdef extern from "bg/date_utilities.hpp" namespace "bondgeek":
    
    # IMM Interfaces
    bool            isIMMdate(_qldate.Date date_ref, bool mainCycle)
    bool            isIMMcode(string instring, bool mainCycle)
    string          imm_code(_qldate.Date date_ref)
    _qldate.Date    imm_date(string immCode, _qldate.Date ref_date)
    _qldate.Date    imm_nextDate(_qldate.Date date_ref, bool mainCycle)
    string          imm_nextCode(string immCode, 
                                 _qldate.Date date_ref,
                                 bool mainCycle)


# IMM Inteface
cdef class IMM:
    @classmethod
    def nextDate(cls, object date_ref=None):
        
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

