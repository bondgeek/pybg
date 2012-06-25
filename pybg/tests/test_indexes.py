"""
 Copyright (C) 2011, Enthought Inc
 Copyright (C) 2011, Patrick Henaff

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
"""

import unittest

from pybg.quantlib.currency import USDCurrency
from pybg.quantlib.index import Index
from pybg.quantlib.indexes.interest_rate_index import InterestRateIndex
from pybg.quantlib.indexes.libor import Libor
from pybg.ql import Settings
from pybg.quantlib.time.api import Days, Months, Period, TARGET
from pybg.quantlib.time.api import Actual360, today

class TestIndex(unittest.TestCase):

    def test_create_index(self):

        with self.assertRaises(ValueError):
            Index()

class TestIRIndex(unittest.TestCase):

    def test_create_index(self):

        with self.assertRaises(ValueError):
            InterestRateIndex()

class TestLibor(unittest.TestCase):

    def test_create_libor_index(self):

        settings = Settings.instance()

        # Market information
        calendar = TARGET()

        # must be a business day
        eval_date = calendar.adjust(today())
        settings.evaluation_date = eval_date


        settlement_days = 2
        settlement_date = calendar.advance(eval_date, settlement_days, Days)
        # must be a business day
        settlement_date = calendar.adjust(settlement_date);

        index = Libor('USD Libor', Period(6, Months), settlement_days,
                        USDCurrency(), calendar, Actual360())

        self.assertEquals('USD Libor6M Actual/360', index.name)

if __name__ == '__main__':
    unittest.main()
