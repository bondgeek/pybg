import unittest

from datetime import date

import pybg.instruments.equityoption as O 

from pybg.settings import Settings
from pybg.enums import (
    DayCounters, Frequencies, BusinessDayConventions, Calendars
    )

    
class EquityOptionsTestCase(unittest.TestCase):

    def test_optionvalue1(self):
        '''
        Hull: ex. 12.2
        
        '''
        Settings().evaluation_date = 20120928
        
        optn = O.EquityOption(300., 20121201, .03)
        ov = optn.calc(.2, 310., .08)
                
        self.assertEqual(date(2012, 10, 1), optn.settlementDate)
        self.assertAlmostEqual(ov, 17.28, 2)
        
        
    def test_gamma(self):
        '''
        Hull: ex. 14.7
        
        '''
        Settings().evaluation_date = 20120731
        
        optn = O.EquityOption(300., 20121201, .03)
        ov = optn.calc(.25, 305, .08)
        
        self.assertAlmostEqual(optn.greeks['gamma'], .00857, 5)