# enumerations used in QuantLib code.
from bgtools.utils.dates import parse_date

from pybg.ql import qldate_from_pydate, pydate_from_qldate

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

class DayCounters(dict):
    from pybg.quantlib.time.daycounter import (
        Thirty360, Actual360, Actual365Fixed
        )
    from pybg.quantlib.time.daycounters.actual_actual import (
        ActualActual, ISMA, ISDA, Bond
        )
        
    _lookup = dict([(dc.name(), dc) for dc in
        [Thirty360(), Actual360(), Actual365Fixed(),
        ActualActual(), ActualActual(ISMA), ActualActual(ISDA),
        ActualActual(Bond)
        ]]
        )
    
    def __init__(self, *args):
        dict.__init__(self, self._lookup)
        self.update(*args)
    
    @classmethod
    def year_fraction(cls, date1, date2, daycounter=None):
        '''
        Calculate the fraction of a year between two date1 and date2, 
        based on the daycount specified.
        
        dates may be ccyymmdd or python datetime.dates
        
        '''
        pydate1, pydate2 = map(parse_date, (date1, date2))
        
        if not daycounter:
            daycounter = cls.ActualActual()
        
        qldate1 = qldate_from_pydate(pydate1)
        qldate2 = qldate_from_pydate(pydate2)

        return daycounter.year_fraction(qldate1, qldate2)
        
    @classmethod
    def daycount(cls, date1, date2, daycounter=None):
        pydate1, pydate2 = map(parse_date, (date1, date2))
        
        if not daycounter:
            daycounter = cls.ActualActual()
        
        qldate1 = qldate_from_pydate(pydate1)
        qldate2 = qldate_from_pydate(pydate2)
        
        return daycounter.day_count(qldate1, qldate2)
            
                
class Calendars(dict):
    '''
    Wrapper for quantlib Calendar objects and methods. Accepts python.datetime
    objects instead of pyql.quantlib dates.
    
    :adjust:            Adjust date to business day
    :advance:           Advance date by specified period
    :is_business_day:   Checks date
    
    can be used as a dict using name property of pyql.quantlib.calendar objects
    for example:
        
        Calendars()['TARGET'] returns TARGET calendar
        
    ''' 

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
        
        qldate = qldate_from_pydate(parse_date(pydate))
        try:
            return pydate_from_qldate(calendar.adjust(qldate, convention))
        except:
            try:
                return pydate_from_qldate(calendar().adjust(qldate, convention))
            except:
                return None


    @classmethod
    def advance(cls, pydate, n, timeunit=None, calendar=None, convention=None):
        """
        Advance pydate according the specified calendar and convention
        
        :pydate:   e.g. 19600809, date(1964, 9, 29), '5-23-1993'
        :n:        integer
        :timeunit: e.g., enums.TimeUnits.Days

        usage
        -----
        
        Note 9/6/1980 is a weekend
        
        >>> Calendars.advance(19800906, 1)
        datetime.date(1980, 9, 8)
        
        """
        if not calendar:
            calendar = cls.TARGET()
        
        elif not hasattr(calendar, "advance"):
            return None
        
        if not convention:
            convention = BusinessDayConventions.Following
        
        if not timeunit:
            timeunit = TimeUnits.Days
            
        qldate = qldate_from_pydate(parse_date(pydate))
        try:
            return pydate_from_qldate(calendar.advance(qldate, n, timeunit))
            
        except:
            try:
                return pydate_from_qldate(
                    calendar().advance(qldate, n, timeunit)
                    )
            
            except:
                print("failure {}".format(qldate))
                return None

    @classmethod
    def is_business_day(cls, pydate, calendar=None):
        if not calendar:
            calendar = cls.TARGET()
        
        elif not hasattr(calendar, "advance"):
            return None
        
        qldate = qldate_from_pydate(parse_date(pydate))
        try:
            return calendar.is_business_day(qldate)
            
        except:
            try:
                return calendar().is_business_day(qldate)
            
            except:
                return None            

if __name__ == "__main__":
    import doctest
    doctest.testmod()