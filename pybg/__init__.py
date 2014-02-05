"""
pybg -- bondgeek's fixed income python libraries
================================================

pybg is a set of wrappers and extensions to QuantLib.  It is inspired by,
and borrows freely from Enthought's PyQL Cython wrappers for QuantLib.

The purpose of this libraries is to:

1) Provide a interface to QuantLib's fixed-income capabilities that is easily 
used in a Python front end.
2) Minimize the extent of new data types needed to communicate with the library. 
For example, python datetime objects are used instead of QuantLib Dates.

Dependencies:
-QuantLib c++ library
-bg, bondgeek c++ wrappers for QuantLib classes
-bgtools module

"""

__version__ = '0.1.0'

__license__ = """
License Info:
    QuantLib: http://quantlib.org/reference/license.html
    pyql:   https://github.com/enthought/pyql/blob/master/LICENSE.txt
    pybg:   MIT License, Copyright (c) 2012 BG Research LLC

"""
DBG = True
if not DBG:
    from pybg.quantlib.currency import USDCurrency, EURCurrency
    from pybg.quantlib.quotes import SimpleQuote

    from pybg.settings import get_eval_date, set_eval_date, Settings

    from pybg.enums import (
        DayCounters, Frequencies, BusinessDayConventions, Calendars,
        qldate_from_pydate, pydate_from_qldate
        )

    from pybg.tenor import Tenor

    bondtypes = dict(
        muni=dict(calendar=Calendars.UnitedStates(Calendars.GOVERNMENTBOND),
                  daycounter=DayCounters.Thirty360(DayCounters.Bond)
                  ),
        ustsy=dict(calendar=Calendars.UnitedStates(Calendars.GOVERNMENTBOND),
                   daycounter=DayCounters.ActualActual()
                   ),
        )


