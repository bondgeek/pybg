import unittest

import pybg.ql
import pybg.curves as curves
import pybg.instruments.bulletbond as bb
import pybg.instruments.sinkingfundbond as sf 

from pybg.enums import (
    DayCounters, Frequencies, BusinessDayConventions, Calendars
    )
    
from datetime import date


class CurvesTestCase(unittest.TestCase):

    def test_bondcurve(self):
        
        pybg.ql.set_eval_date(date(2008, 9, 15))
 
        bcrv = curves.BondCurve( curves.CurveBase(
            Calendars.UnitedStates(Calendars.GOVERNMENTBOND),
            3,
            DayCounters.Actual360(),
            Frequencies.Semiannual,
            BusinessDayConventions.Unadjusted,
            DayCounters.ActualActual(DayCounters.Bond),
            DayCounters.ActualActual(DayCounters.ISDA) )
            )
         
        dated = [date(2005, 3, 15),
                date(2005, 6, 15),
                date(2006, 6, 30),
                date(2002, 11, 15),
                date(1987, 5, 15)
                ]
            
        maturities = [
                date(2010, 8, 31),
                date(2011, 8, 31),
                date(2013, 8, 31),
                date(2018, 8, 15),
                date(2038, 5, 15)
                ]
            
        couponRates = [
                0.02375,
                0.04625,
                0.03125,
                0.04000,
                0.04500
                ]
            
        marketQuotes = [
                100.390625,
                106.21875,
                100.59375,
                101.6875,
                102.140625
                ]
                
        bond_ids = ["B1", "B2", "B3", "B4", "B5"]

        depos = {
                "3M": 0.0096,
                "6M": 0.0145,
                "1Y": 0.0194
                }

        q = zip(marketQuotes, maturities, couponRates, dated)
        bndcrv = dict(zip(bond_ids, q))

        bcrv.update(bndcrv, depos)

        for id, bndrow in bndcrv.items():
            qt, mty, cpn, dtd = bndrow
            b = bb.BulletBond(cpn, 
                              mty, 
                              dtd, 
                              Calendars.UnitedStates(Calendars.GOVERNMENTBOND))
            b.setEngine(bcrv)
            self.assertAlmostEqual(qt, b.toPrice(), 5)

    def test_bulletbond(self):
        
        pybg.ql.set_eval_date(date(2012,8,28))
 
        dated = date(2003, 5, 15)
        mty = date(2027, 5, 15)

        bnd = bb.BulletBond(.06, 
                            date(2022, 8, 31), 
                            date(2012, 8, 31), 
                            Calendars.UnitedStates(Calendars.GOVERNMENTBOND))

        self.assertAlmostEqual(bnd.toPrice(.06), 100.0, 5)
        self.assertAlmostEqual(bnd.toPrice(.08), 86.409674,5)
        
    def test_sinkingfundbond(self):

        pybg.ql.set_eval_date(date(2012,6,28))
 
        dated = date(2003, 6, 15)
        maturities = [
            date(2027, 6, 1),
            date(2026, 6, 1),
            date(2025, 6, 1)
            ]
        
        bullet_prices = []
        for mty in maturities: 
            bnd = bb.BulletBond(.05, 
                mty, 
                dated, 
                daycounter=DayCounters.Thirty360(DayCounters.Bond),
                calendar=Calendars.UnitedStates(Calendars.US_SETTLEMENT)
                )
                            
            bullet_prices.append(bnd.toPrice(.06))
            
        sfbnd = sf.SinkingFundBond(
            .05, 
            maturities[0], 
            (40., 40., 40.), 
            Frequencies.Annual,
            dated,
            daycounter=DayCounters.Thirty360(DayCounters.Bond),
            calendar=Calendars.UnitedStates(Calendars.US_SETTLEMENT)
            )
            
        avgprc = sum(bullet_prices)/len(bullet_prices)
        sfprc = sfbnd.toPrice(.06)

        self.assertAlmostEqual(avgprc, sfprc, 5)
