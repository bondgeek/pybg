import pybg.ql 
import pybg.enums
import pybg.curves as curves

import pybg.instruments.bulletbond as bb
import pybg.instruments.sinkingfundbond as sf 

from datetime import date

dt0 = date(2012, 6, 30)

depos = {
'12M': 0.010695,
 '1D': 0.00169,
 '1M': 0.0024875,
 '1W': 0.001996,
 '3M': 0.004561,
 '6M': 0.007344}


print("\nSetting eval date: %s" % dt0)
pybg.ql.set_eval_date(dt0)


govbondcurve = curves.CurveBase(pybg.enums.Calendars.UnitedStates(pybg.enums.Calendars.GOVERNMENTBOND),
1,
pybg.enums.DayCounters.Actual360(),
pybg.enums.Frequencies.Semiannual,
pybg.enums.BusinessDayConventions.ModifiedFollowing,
pybg.enums.DayCounters.ActualActual(pybg.enums.DayCounters.Bond),
pybg.enums.DayCounters.ActualActual(pybg.enums.DayCounters.ISDA)
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

q = zip(marketQuotes, maturities, couponRates, dated)
bndcrv = dict(zip(bond_ids, q))

bcrv.update(bndcrv)

#bulletbond

uscal = pybg.enums.Calendars.UnitedStates(pybg.enums.Calendars.GOVERNMENTBOND)

dated = date(2003, 5, 15)
mty = date(2027, 5, 15)

bnd1 = bb.BulletBond(.06, mty, dated, uscal)

#sinker
sfbnd = sf.SinkingFundBond(.06, 
                            mty, 
                            (40., 40., 40.), 
                            pybg.enums.Frequencies.Annual,
                            dated)
                            