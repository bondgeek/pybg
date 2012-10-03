"""
 Copyright (C) 2011, Enthought Inc
 Copyright (C) 2011, Patrick Henaff

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
"""

import unittest

from pybg.settings import Settings
from pybg.ql import qldate_from_pydate, pydate_from_qldate

from pybg.quantlib.currency import USDCurrency

from pybg.quantlib.termstructures.yields.rate_helpers import DepositRateHelper, SwapRateHelper
from pybg.quantlib.termstructures.yields.piecewise_yield_curve import \
    term_structure_factory
from pybg.quantlib.time.api import Date, TARGET, Period, Months, Years, Days
from pybg.quantlib.time.api import September, ISDA, today
from pybg.quantlib.time.api import ModifiedFollowing, Unadjusted, Actual360
from pybg.quantlib.time.api import Thirty360, ActualActual, Actual365Fixed
from pybg.quantlib.time.api import Annual
from pybg.quantlib.quotes import SimpleQuote

from pybg.quantlib.indexes.libor import Libor

class PiecewiseYieldCurveTestCase(unittest.TestCase):

    def test_creation(self):

        # Market information
        settings = Settings(calendar = TARGET())
        calendar = settings.calendar 

        settlement_date = Date(18, September, 2008)
        # must be a business day
        settlement_date = calendar.adjust(settlement_date);

        # must be a business day
        settings.evaluation_date = pydate_from_qldate(
            calendar.advance(settlement_date, -2, Days)
            )

        quotes = [0.0096, 0.0145, 0.0194]
        tenors =  [3, 6, 12]

        rate_helpers = []

        deposit_day_counter = Actual365Fixed()
        convention = ModifiedFollowing
        end_of_month = True

        for quote, month in zip(quotes, tenors):
            tenor = Period(month, Months)
            fixing_days = 3

            helper = DepositRateHelper(
                quote, tenor, fixing_days, calendar, convention, end_of_month,
                deposit_day_counter
            )

            rate_helpers.append(helper)


        ts_day_counter = ActualActual(ISDA)

        tolerance = 1.0e-15

        ts = term_structure_factory(
            'discount', 'loglinear', settlement_date, rate_helpers,
            ts_day_counter, tolerance
        )

        self.assertIsNotNone(ts)

        self.assertEquals( Date(18, September, 2008), ts.reference_date)

        # this is not a real test ...
        self.assertAlmostEquals(0.9975, ts.discount(Date(21, 12, 2008)), 4)
        #self.assertAlmostEquals(0.9944, ts.discount(Date(21, 4, 2009)), 4)
        #self.assertAlmostEquals(0.9904, ts.discount(Date(21, 9, 2009)), 4)

    def test_deposit_swap(self):

        # Market information
        settings = Settings(calendar = TARGET())
        calendar = settings.calendar 

        # must be a business day
        eval_date = qldate_from_pydate(settings.today())
        
        settlement_days = 2
        settlement_date = calendar.advance(eval_date, settlement_days, Days)
        # must be a business day
        settlement_date = calendar.adjust(settlement_date);

        depositData = [[ 1, Months, 4.581 ],
                       [ 2, Months, 4.573 ],
                       [ 3, Months, 4.557 ],
                       [ 6, Months, 4.496 ],
                       [ 9, Months, 4.490 ]]

        swapData = [[ 1, Years, 4.54 ],
                    [ 5, Years, 4.99 ],
                    [ 10, Years, 5.47 ],
                    [ 20, Years, 5.89 ],
                    [ 30, Years, 5.96 ]]

        rate_helpers = []

        end_of_month = True

        for m, period, rate in depositData:
            tenor = Period(m, Months)

            helper = DepositRateHelper(rate/100, tenor, settlement_days,
                     calendar, ModifiedFollowing, end_of_month,
                     Actual360())

            rate_helpers.append(helper)

        liborIndex = Libor('USD Libor', Period(6, Months), settlement_days,
                           USDCurrency(), calendar, Actual360())

        spread = SimpleQuote(0)
        fwdStart = Period(0, Days)

        for m, period, rate in swapData:
            rate = SimpleQuote(rate/100)

            helper = SwapRateHelper(rate, Period(m, Years),
                calendar, Annual,
                Unadjusted, Thirty360(),
                liborIndex, spread, fwdStart)

            rate_helpers.append(helper)

        ts_day_counter = ActualActual(ISDA)
        tolerance = 1.0e-15

        ts = term_structure_factory(
            'discount', 'loglinear', settlement_date, rate_helpers,
            ts_day_counter, tolerance)

        # this is not a real test ...
        self.assertAlmostEquals(0.9103,
             ts.discount(calendar.advance(today(), 2, Years)),3)
        self.assertAlmostEquals(0.7836,
             ts.discount(calendar.advance(today(), 5, Years)),3)
        self.assertAlmostEquals(0.5827,
             ts.discount(calendar.advance(today(), 10, Years)),3)
        self.assertAlmostEquals(0.4223,
             ts.discount(calendar.advance(today(), 15, Years)),3)


if __name__ == '__main__':
    unittest.main()
