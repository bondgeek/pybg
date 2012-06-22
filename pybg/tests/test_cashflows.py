import unittest

from pybg.quantlib.time.date import Date

import pybg.quantlib.cashflow as cf

class TestQuantLibDate(unittest.TestCase):

    def test_today(self):

        cf_date = Date(1, 7, 2030) 
        cf_amount = 100.0
        
        test_cf = cf.SimpleCashFlow(cf_amount, cf_date)

        self.assertEquals(test_cf.amount, cf_amount)
        self.assertEquals(test_cf.date, cf_date)
        