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

