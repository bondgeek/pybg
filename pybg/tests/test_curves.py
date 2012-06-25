import unittest

import pybg.curves as curves

from pybg.ql import get_eval_date, set_eval_date

from datetime import date

class CurvesTestCase(unittest.TestCase):

    def test_ratehelpercurve(self):
        
        # 1/21/2012 is a weekend, should be adjusted to 23rd
        evaldate = date(2012, 1, 21)
        
        depos = dict([(str(n)+"M", .05) for n in [1, 3, 6, 9, 12]])
        swaps = dict([(str(n)+"Y", .05) for n in [2, 3, 5, 7, 10, 12, 15, 20, 30]])
        
        rh = curves.RateHelperCurve("3M")
        
        rh.update(depos, swaps, evaldate)

        self.assertTrue(
            rh.settlementDate == date(2012, 1, 25)
        )

        self.assertTrue(
            rh.tenorquote("10Y") == .05
        )
        
        self.assertTrue(
            rh.discount(0.0) == 1.0
        )    
        
        
        self.assertTrue(
            rh.discount(10.0) <= rh.discount(5.0)
        )    