import pybg.ql 
import pybg.curves as curves

import pybg.instruments.bulletbond as bb
import pybg.instruments.sinkingfundbond as sf 

from pybg.enums import (
    DayCounters, Frequencies, BusinessDayConventions, Calendars
    )
    
from datetime import date

dt0 = date(2008, 9, 15)

print("\nSetting eval date: %s" % dt0)
pybg.ql.set_eval_date(dt0)

govbondcurve = curves.CurveBase(
Calendars.UnitedStates(Calendars.GOVERNMENTBOND),
3,
DayCounters.Actual360(),
Frequencies.Semiannual,
BusinessDayConventions.Unadjusted,
DayCounters.ActualActual(DayCounters.Bond),
DayCounters.ActualActual(DayCounters.ISDA)
)

bcrv = curves.BondCurve(govbondcurve)
 
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
        
bond_ids = [
        "B1", "B2", "B3", "B4", "B5"
    ]

depos = {
        "3M": 0.0096,
        "6M": 0.0145,
        "1Y": 0.0194}

q = zip(marketQuotes, maturities, couponRates, dated)
bndcrv = dict(zip(bond_ids, q))

print("build bond curve...")
bcrv.update(bndcrv, depos)

output_line1 = "bond: {}, price/yield: {:7.3f}/{:6.3f}%"
output_line2 = "      check {:7.3f} vs {:7.3f}"
for id, bndrow in bndcrv.items():
    qt, mty, cpn, dtd = bndrow
    b = bb.BulletBond(cpn, mty, dtd, Calendars.UnitedStates(Calendars.GOVERNMENTBOND))
    print(output_line1.format(id, qt, 100.0*b.toYield(qt)))
    b.setEngine(bcrv)
    print(output_line2.format(qt, b.toPrice()))

#bulletbond
print("test bond")
dated = date(2003, 5, 15)
mty = date(2027, 5, 15)

bnd1 = bb.BulletBond(.06, mty, dated, Calendars.UnitedStates(Calendars.GOVERNMENTBOND))

#sinker
print("sinking fund bond")
sfbnd = sf.SinkingFundBond(.06, 
                            mty, 
                            (40., 40., 40.), 
                            Frequencies.Annual,
                            dated)
                            