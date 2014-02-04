# 
#  

import datetime  

from bgtools.utils.dates import parse_date
from bgtools.utils.dpatterns import Singleton, Record

from pybg.ql import qldate_from_pydate, pydate_from_qldate
from pybg.ql cimport _pydate_from_qldate, _qldate_from_pydate

from pybg.enums import (
    BusinessDayConventions, Calendars
    )
    
from pybg.version import version


cdef extern from "bg/date_utilities.hpp" namespace "bondgeek":
    _qldate.Date get_evaluation_date()
    void set_evaluation_date(_qldate.Date& date)

class TaxRates(Record):
    _default ={'cap_gains': .238,
               'ord_income': .434,
               'demin_ptsyear': .25
               }
    def __init__(self, values=None):
        if not values:
            values = self._default
        Record.__init__(self, values.keys(), values)
    
# Global Settings
#    Using datetime objects for interface with the real world
cdef class Settings:

    def __init__(self, 
                 calendar=Calendars.TARGET(), 
                 convention=BusinessDayConventions.Following,
                 taxrates=TaxRates()):
                 
        self._calendar = calendar
        self._convention = convention
        self._taxrates = taxrates
    
    def today(self):
        self.evaluation_date = datetime.date.today()
        return self.evaluation_date
        
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
                self._calendar,
                self._convention
                )
                
            cdef _qldate.Date date_ref = _qldate_from_pydate(evaluation_date)
            set_evaluation_date(date_ref)
    
    property calendar:
        '''market calendar'''
        def __get__(self):
            return self._calendar
            
    property version:
        """Returns the QuantLib C++ version (QL_VERSION) used by this wrapper.
        
        """
        def __get__(self):
            return version()
    
    property taxrates:
        def __get__(self):
            return self._taxrates
        
        def __set__(self, newrates):
            values = {}
            for a in TaxRates.attrs:
                try:
                    v = newrates.get(a, None)
                except:
                    v = getattr(newrates, a, None)
                    
                if v:
                    values[a] = v
            

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

class PySettings(Singleton):
    instance = Settings()
        
        