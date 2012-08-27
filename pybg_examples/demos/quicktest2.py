
import pybg.enums as enums

import pybg.curves as curves
import pybg.instruments.bulletbond as B

from pybg.curvetypes import USDLiborCurve, EURiborCurve

from pybg.instruments.fixedfloatswap import USDLiborSwap, FixedPayer, FixedReceiver, EuriborSwap

from pybg.ql import get_eval_date, set_eval_date

import pybg.ql as ql

from datetime import date

import pybg

fn = pybg.__file__
print("File: %s " % fn)

evaldate = date(2004, 9, 20)

set_eval_date(evaldate)
print("date: {}".format(get_eval_date()))

futtenors, futspots = (
    ("ED1", "ED2", "ED3", "ED4", "ED5", "ED6", "ED7", "ED8"),
    (96.2875, 96.7875, 96.9875, 96.6875, 96.4875, 96.3875, 96.2875, 96.0875)
)
depotenors, depospots = (
    ("1W", "1M", "3M", "6M", "9M", "1y"),   
    (.0382, 0.0372, 0.0363, 0.0353, 0.0348, 0.0345)
)
swaptenors, swapspots = (
    ("2y", "3y", "5y", "10Y", "15Y"),
    (0.037125, 0.0398, 0.0443, 0.05165, 0.055175)
)

futures = dict(zip(futtenors, futspots))
depos = dict(zip(depotenors, depospots))
swaps = dict(zip(swaptenors, swapspots))

print("one")
rh = curves.RateHelperCurve(EURiborCurve("6M", pybg.enums.Frequencies.Annual))

print("two")
dt = rh.update(depos, {}, swaps, evaldate)

print("\nin quicktest curve: \ncrv date: %s" % dt)
print("ref date: %s" % rh.referenceDate)

df = rh.discount(10.0)
print("\nTest curve: \ndiscount: %s" % df)

#BONDS
uscal = enums.Calendars.UnitedStates(enums.Calendars.GOVERNMENTBOND)

print("\n\nbonding: {}".format(get_eval_date()))

bb = B.BulletBond(0.045, date(2017, 5, 15), date(2003, 5, 15), uscal)
bb.setEngine(rh)
print("\n\ncalc...")
prc = bb.toPrice()
yld = bb.toYield(prc)
print("\n\nBullet bond value: {0:.3f}, {1:.3%}".format(prc, yld))
