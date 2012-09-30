# 
#  

import datetime  

from bgtools.utils.dates import parse_date

from pybg.ql import qldate_from_pydate, pydate_from_qldate
from pybg.ql cimport _pydate_from_qldate, _qldate_from_pydate

from pybg.enums import (
    BusinessDayConventions, Calendars
    )
    
from pybg.version import version


cdef extern from "bg/date_utilities.hpp" namespace "bondgeek":
    _qldate.Date get_evaluation_date()
    void set_evaluation_date(_qldate.Date& date)


# Global Settings
#    Using datetime objects for interface with the real world
cdef class Settings:

    def __init__(self, 
                 calendar=Calendars.TARGET(), 
                 convention=BusinessDayConventions.Following):
                 
        self.calendar = calendar
        self.convention = convention
        
    property evaluation_date:
        """
        Property to set/get the evaluation date. 
        
        """
        def __get__(self):
            cdef _qldate.Date evaluation_date = get_evaluation_date()
            self.evaluation_date = _pydate_from_qldate(evaluation_date)
            
            return _pydate_from_qldate(get_evaluation_date())

        def __set__(self, evaluation_date):
            evaluation_date = Calendars.adjust(
                parse_date(evaluation_date),
                self.calendar,
                self.convention
                )
                
            cdef _qldate.Date date_ref = _qldate_from_pydate(evaluation_date)
            set_evaluation_date(date_ref)
            
    property version:
        """Returns the QuantLib C++ version (QL_VERSION) used by this wrapper.
        
        """
        def __get__(self):
            return version()

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
