import unittest

from pybg.quantlib.time.api import *

import pybg.curves as curves
from pybg.curvetypes import USDLiborCurve, EURiborCurve

from pybg.settings import get_eval_date, set_eval_date

import pybg.ql as ql
import pybg.imm as imm

from datetime import date

class CurvesTestCase(unittest.TestCase):

    def test_ratehelpercurve(self):
        
        # 1/21/2012 is a weekend, should be adjusted to 23rd
        evaldate = date(2012, 1, 21)
        
        #set_eval_date(evaldate)
        
        depos = dict([(str(n)+"M", .05) for n in [1, 3, 6, 9, 12]])
        swaps = dict([(str(n)+"Y", .05) for n in [2, 3, 5, 7, 10, 12, 15, 20, 30]])
        futures = {}
        
        rh = curves.RateHelperCurve(USDLiborCurve("3M"))

        rh.update(depos, futures, swaps, evaldate)

        self.assertTrue(
            rh.referenceDate == date(2012, 1, 25)
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

    def test_euriborcurve(self):
        
        # 1/21/2012 is a weekend, should be adjusted to 23rd
        evaldate = date(2012, 1, 21)
        
        #set_eval_date(evaldate)
        
        depos = dict([(str(n)+"M", .05) for n in [1, 3, 6, 9, 12]])
        swaps = dict([(str(n)+"Y", .05) for n in [2, 3, 5, 7, 10, 12, 15, 20, 30]])
        futures = {}
        
        rh = curves.RateHelperCurve(EURiborCurve("6M", Annual))

        rh.update(depos, futures, swaps, evaldate)
        
        print("Test EURibor")
        self.assertTrue(
            rh.referenceDate == date(2012, 1, 25)
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
        
        print("Done\n\n")
    
    def test_IMM(self):
        
        set_eval_date(date(2012, 7, 2))
        
        cdate = imm.IMM.contract_date('ED3')
        
        self.assertTrue(
            cdate == date(2013, 3, 20)
        )
    
    def test_swapcurve(self):
    
        evaldate = date(2004, 9, 20)
        
        set_eval_date(evaldate)
        
        futures = dict(zip(
       ("ED1", "ED2", "ED3", "ED4", "ED5", "ED6", "ED7", "ED8"),
       (96.2875, 96.7875, 96.9875, 96.6875, 96.4875, 96.3875, 96.2875, 96.0875)
        ))
        depos = dict(zip(
        ("1W", "1M"),
        (.0382, 0.0372)
        ))
        swaps = dict(zip(
        ("3y", "5y", "10Y", "15Y"),
        (0.0398, 0.0443, 0.05165, 0.055175)
        ))
        
        rh = curves.RateHelperCurve(EURiborCurve("6M", Annual))

        rh.update(depos, futures, swaps, evaldate)

        self.assertTrue(
            abs(rh.discount(10.) - 0.597) < 0.001
            )