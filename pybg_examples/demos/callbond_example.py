from pybg.settings import PySettings, set_eval_date

from pybg.enums import Frequencies

import pybg.curves as curves
from pybg.curvetypes import USDLiborCurve, EURiborCurve

from datetime import date

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

rh = curves.RateHelperCurve(USDLiborCurve("6M", Frequencies.Semiannual))

rh.update(depos, {}, swaps, evaldate)
df = rh.discount(10.0)



#BONDS
import pybg.enums as enums
uscal = enums.Calendars.UnitedStates(enums.Calendars.GOVERNMENTBOND)

print("\n\nCallable Bond")

cb = C.CallBond(.045, 
                 date(2017, 5, 15), 
                 date(2016, 5, 15), 100., date(2017, 5, 15),
                 date(2003, 5, 15), 
                 .0475,
                 calendar=uscal
                 )

cb.oasEngine(rh, 0., .2017, True)

prc_cb = cb.oasValue(0.0, .2017)

print("Call bond value: {0:.3f}".format(prc_cb))

prc = cb.toPrice()
yld = cb.toYield(prc)
print("Call bond value: {0:.3f}, {1:.3%}".format(prc, yld))
print("After-tax yield: {:.3%}".format(cb.toATY(prc)))
