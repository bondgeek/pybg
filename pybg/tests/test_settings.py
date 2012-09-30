import unittest

from pybg.quantlib.time.api import (
    Date, Days, August, January, Period, Jul, Annual, today, Years, TARGET,
    Unadjusted, Schedule, ModifiedFollowing, Backward, ActualActual, ISMA,
    Following
)

from pybg.ql import pydate_from_qldate
from pybg.settings import Settings, get_eval_date, set_eval_date

from datetime import date

class SettingsTestCase(unittest.TestCase):

    def test_using_settings(self):

        settings = Settings()

        ql_date_today = Date(12, January, 2012)

        evaluation_date = pydate_from_qldate(ql_date_today)
        
        # have to set the evaluation date before the test as it is a global
        # attribute for the whole library ... meaning that previous test_cases
        # might have set this to another date
        settings.evaluation_date = evaluation_date

        self.assertTrue(
            evaluation_date == settings.evaluation_date
        )

        self.assertTrue(settings.version.startswith('1'))

    def test_settings_instance_method(self):

        Settings.instance().evaluation_date = date(2012, 6, 21)

        self.assertEquals(
                date(2012, 6, 21),
                Settings.instance().evaluation_date
        )
        
    def test_settings_functional(self):
        
        dt = date(1960, 8, 9)
        
        set_eval_date(dt)
        
        self.assertEquals(
            dt,
            get_eval_date()
            )
            
    def test_settings_null(self):
        
        set_eval_date()
        
        #if you run on the weekend, date will roll forward
        self.assertTrue(
            get_eval_date() >= date.today()
            )
        
