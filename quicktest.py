from pybg import Annual, Semiannual

import pybg.curves as curves
from pybg.curvetypes import USDLiborCurve, EURiborCurve

from pybg.instruments.fixedfloatswap import USDLiborSwap, FixedPayer, FixedReceiver, EuriborSwap

from pybg.ql import get_eval_date, set_eval_date

import pybg.ql as ql

from datetime import date


evaldate = date(2004, 9, 20)

set_eval_date(evaldate)

futtenors, futspots = (
    ("ED1", "ED2", "ED3", "ED4", "ED5", "ED6", "ED7", "ED8"),
    (96.2875, 96.7875, 96.9875, 96.6875, 96.4875, 96.3875, 96.2875, 96.0875)
)
depotenors, depospots = (
    ("1W", "1M", "3M", "6M", "9M", "1y"),   
    (.0382, 0.0372, 0.0363, 0.0353, 0.0348, 0.0345)
)
swaptenors, swapspots = (
    ("3y", "5y", "10Y", "15Y"),
    (0.0398, 0.0443, 0.05165, 0.055175)
)

futures = dict(zip(futtenors, futspots))
depos = dict(zip(depotenors, depospots))
depo2 = dict(zip(depotenors[:2], depospots[:2]))
swaps = dict(zip(swaptenors, swapspots))

rh = curves.RateHelperCurve(EURiborCurve("6M", Annual))

rh.update(depos, {}, swaps, evaldate)

print("\nTest curve: \ndate: %s" % rh.referenceDate)

df = rh.discount(10.0)
print("\nTest curve: \ndiscount: %s" % df)

swp = USDLiborSwap("6M", rh.referenceDate, date(2009, 9, 22), .04, FixedPayer)
swp2 = EuriborSwap("6M", rh.referenceDate, date(2009, 9, 22), .04, FixedPayer)

swp2.setEngine(rh)

fxNPV = 0.0
for amt, dt in swp2.fixedLeg:
    fxDF = rh.discount(dt)
    fxNPV += fxDF*amt
    print("cf: %10s %12.2f %11.8f %12.3f" % (dt, amt, fxDF, fxNPV))

    