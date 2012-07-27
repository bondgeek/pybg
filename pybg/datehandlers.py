
from pybg.enums import BusinessDayConventions
from pybg.ql import qldate_from_pydate, pydate_from_qldate

class Calendars(dict):
    from pybg.quantlib.time.calendar import TARGET
    from pybg.quantlib.time.calendars.null_calendar import NullCalendar
    from pybg.quantlib.time.calendars.germany import (
        Germany, EUREX, FrankfurtStockExchange, SETTLEMENT as GER_SETTLEMENT,
        EUWAX, XETRA
        )
    from pybg.quantlib.time.calendars.united_kingdom import (
        EXCHANGE, METALS, SETTLEMENT as UK_SETTLEMENT, 
        UnitedKingdom
        ) 
    from pybg.quantlib.time.calendars.united_states import (
        UnitedStates, GOVERNMENTBOND, NYSE, NERC, SETTLEMENT as US_SETTLEMENT
        )
    
    
    _lookup = dict([(cal.name(), cal) for cal in 
        [TARGET(), NullCalendar(),
         Germany(), Germany(EUREX), Germany(FrankfurtStockExchange), 
         Germany(GER_SETTLEMENT), Germany(EUWAX), Germany(XETRA),
         UnitedKingdom(),
         UnitedKingdom(EXCHANGE), UnitedKingdom(METALS), 
         UnitedKingdom(UK_SETTLEMENT),
         UnitedStates(),
         UnitedStates(GOVERNMENTBOND), UnitedStates(NYSE), UnitedStates(NERC), 
         UnitedStates(US_SETTLEMENT) ]
        ]
        )
    
    def __init__(self, *args):
        dict.__init__(self, self._lookup)
        self.update(*args)

    @classmethod
    def adjust(cls, pydate, calendar=None, convention=None):
        if not calendar:
            calendar = cls.TARGET()
            
        elif not hasattr(calendar, "adjust"):
            return None
        
        if not convention:
            convention = BusinessDayConventions.Following
        
        qldate = qldate_from_pydate(pydate)
        try:
            return pydate_from_qldate(calendar.adjust(qldate, convention))
        except:
            try:
                return pydate_from_qldate(calendar().adjust(qldate, convention))
            except:
                return None


    @classmethod
    def advance(cls, pydate, n, timeunit, calendar=None, convention=None):
        if not calendar:
            calendar = cls.TARGET()
        
        elif not hasattr(calendar, "advance"):
            return None
        
        if not convention:
            convention = BusinessDayConventions.Following
        
        qldate = qldate_from_pydate(pydate)
        try:
            return pydate_from_qldate(calendar.advance(qldate, n, timeunit))
            
        except:
            try:
                return pydate_from_qldate(calendar().advance(qldate, n, timeunit))
            
            except:
                return None

    @classmethod
    def is_business_day(cls, pydate, calendar=None):
        if not calendar:
            calendar = cls.TARGET()
        
        elif not hasattr(calendar, "advance"):
            return None
        
        qldate = qldate_from_pydate(pydate)
        try:
            return calendar.is_business_day(qldate)
            
        except:
            try:
                return calendar().is_business_day(qldate)
            
            except:
                return None            