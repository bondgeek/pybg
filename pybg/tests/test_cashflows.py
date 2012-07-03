import unittest

from datetime import date

from pybg.quantlib.time.date import Date

import pybg.quantlib.cashflow as cf

class TestQuantLibDate(unittest.TestCase):

    def test_today(self):

        pydate = date(2030, 7, 1)

        cf_date = Date(1, 7, 2030)
        cf_amount = 100.0
        
        test_cf = cf.SimpleCashFlow(cf_amount, cf_date)

        self.assertEquals(test_cf.amount, cf_amount)
        self.assertEquals(test_cf.date, cf_date)
        
        leg = ( (cf_amount, cf_date), )
        
        test_leg = cf.SimpleLeg(leg)
        
        self.assertEquals(test_leg.size,  1)
        self.assertEquals(test_leg.items, [(100.0, pydate)])
        