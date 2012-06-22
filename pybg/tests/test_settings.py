import unittest

from pybg.quantlib.time.api import (
    Date, Days, August, Period, Jul, Annual, today, Years, TARGET,
    Unadjusted, Schedule, ModifiedFollowing, Backward, ActualActual, ISMA,
    Following
)

from pybg.ql import Settings, pydate_from_qldate, get_eval_date, set_eval_date

from datetime import date

class SettingsTestCase(unittest.TestCase):

    def test_using_settings(self):

        settings = Settings()

        ql_date_today = today()

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

        Settings.instance().evaluation_date = date.today()

        self.assertEquals(
                date.today(),
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
        
        self.assertEquals(
            date.today(),
            get_eval_date()
            )
        
