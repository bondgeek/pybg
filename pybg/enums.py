# enumerations used in QuantLib code.
    
class TimeUnits(object):
    from pybg.quantlib.time.date import Days, Weeks, Months, Years

class Compounding(object):
    Simple = 0     # 1+rt
    Compounded = 1 # (1+r)^t
    Continuous = 2 # e^{rt}
    SimpleThenCompounded = 3 # Simple up to the first period then Compounded

class BusinessDayConventions(object):
    from pybg.quantlib.time.calendar import (
        ModifiedFollowing, Following, ModifiedPreceding,
        Preceding, Unadjusted, 
    )
    
class Frequencies(object):
    from pybg.quantlib.time.date import (
        Annual, Semiannual, Quarterly, Monthly, Weekly, Daily
    )

class Months(object):
    from pybg.quantlib.time.date import (
        January, February, March, April, May, June, 
        July, August, September, November, December,
        Jan, Feb, Mar, Apr, Jun, Jul, Aug, Sep, Oct, Nov, Dec
    )
    
class DateGeneration(object):
    from pybg.quantlib.time.schedule import Backward, Forward


class SwapPayType(object):
    from pybg.instruments.fixedfloatswap import FixedPayer, FixedReceiver

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
