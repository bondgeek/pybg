# TODO:  add Calendars class, which will be a lookup table for 
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
    def advance(cls, pydate, n, timeunit, calendar=None):
        if not calendar:
            calendar = cls.TARGET()
        elif not hasattr(calendar, "advance"):
            return None
        
        try:
            return calendar.advance(pydate, n, timeunit)
        except:
            try:
                return calendar().advance(pydate, n, timeunit)
            except:
                return None
            
        