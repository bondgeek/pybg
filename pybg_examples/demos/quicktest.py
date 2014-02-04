
from datetime import date

import pybg.curves as curves

from pybg import get_eval_date, set_eval_date
from pybg.enums import Frequencies, DayCounters, Calendars
from pybg.curvetypes import USDLiborCurve, EURiborCurve
from pybg.instruments.fixedfloatswap import USDLiborSwap, FixedPayer, FixedReceiver, EuriborSwap

import pybg.instruments.bulletbond as B
import pybg.instruments.callbond as C

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
    ("2y", "3y", "5y", "10Y", "15Y"),
    (0.037125, 0.0398, 0.0443, 0.05165, 0.055175)
)

futures = dict(zip(futtenors, futspots))
depos = dict(zip(depotenors, depospots))
swaps = dict(zip(swaptenors, swapspots))

rh = curves.RateHelperCurve(EURiborCurve("6M", Frequencies.Annual))

rh.update(depos, {}, swaps, evaldate)
df = rh.discount(10.0)

print("\nTest curve: \ndate: %s" % rh.referenceDate)
print("\nTest curve: \ndiscount: %s" % df)

swp2 = EuriborSwap("6M", rh.referenceDate, date(2009, 9, 22), .04, FixedPayer)

swp2.setEngine(rh)

print("\nSwap\nNPV: %s | fx: %s | fl: %s" % (swp2.NPV, swp2.fixedLegNPV, swp2.floatingLegNPV))
print("Fixed Leg")
cfNPV = 0.0
for amt, dt in swp2.fixedLeg:
    cfDF = rh.discount(dt)
    cfNPV += cfDF*amt
    print("cf: %10s %12.2f %11.8f %12.3f" % (dt, amt, cfDF, cfNPV))

print("Floating Leg")
cfNPV = 0.0
for amt, dt in swp2.floatingLeg:
    cfDF = rh.discount(dt)
    cfNPV += cfDF*amt
    print("cf: %10s %12.2f %11.8f %12.3f" % (dt, amt, cfDF, cfNPV))


#BONDS

uscal = Calendars.UnitedStates(Calendars.GOVERNMENTBOND)
dc30360 = DayCounters.Thirty360(DayCounters.Bond)


bb = B.BulletBond(.045, date(2017, 5, 15), date(2003, 5, 15), 
                  calendar=uscal,
                  daycounter=dc30360)
bb.setEngine(rh)

prc = bb.toPrice()
yld = bb.toYield(prc)
print("Bullet bond value: {0:.3f}, {1:.3%}".format(prc, yld))

print("\n\nCallable")


cb = C.CallBond(.045, 
                 date(2017, 5, 15), 
                 date(2016, 5, 15), 100., date(2017, 5, 15),
                 date(2003, 5, 15), 
                 .0475,
                 calendar=uscal,
                  daycounter=dc30360
                 )

cb.oasEngine(rh, 0., .2017, True)

prc_cb = cb.oasValue(0.0, .2017)

print("Call bond value: {0:.3f}".format(prc_cb))


prc = cb.toPrice()
yld = cb.toYield(prc)
print("Call bond value: {0:.3f}, {1:.3%}".format(prc, yld))

print("\n\nreset date and try after-tax")
set_eval_date(20130130)
print("date: {}\ncusip: 64966FMG6".format(get_eval_date()))

nyc = C.CallBond(.05, 
                 date(2016, 9, 1), 
                 date(2015, 9, 1), 100., date(2015,9,1), 
                 date(2005,9,22), 
                 .04, 
                 calendar=uscal,
                 daycounter=dc30360
                 )

atyPx = nyc.atyToPrice(.07, capgains=.238, ordinc=.434)

print("7% aty to price (bbg=90.561): {}".format(atyPx))
