from pybg import Annual, Semiannual

import pybg.curves as curves
from pybg.curvetypes import USDLiborCurve, EURiborCurve

from pybg.instruments.fixedfloatswap import USDLiborSwap, FixedPayer, FixedReceiver, EuriborSwap

from pybg.ql import get_eval_date, set_eval_date

import pybg.ql as ql

from datetime import date


evaldate = date(2004, 9, 20)

set_eval_date(evaldate)

futures = dict(zip(
("ED1", "ED2", "ED3", "ED4", "ED5", "ED6", "ED7", "ED8"),
(96.2875, 96.7875, 96.9875, 96.6875, 96.4875, 96.3875, 96.2875, 96.0875)
))
depos = dict(zip(
("1W", "1M"),
(.0382, 0.0372)
))
swaps = dict(zip(
("3y", "5y", "10Y", "15Y"),
(0.0398, 0.0443, 0.05165, 0.055175)
))

rh = curves.RateHelperCurve(EURiborCurve("6M", Annual))

rh.update(depos, futures, swaps, evaldate)

print("\nTest curve: \ndate: %s" % rh.referenceDate)

df = rh.discount(10.0)
print("\nTest curve: \ndiscount: %s" % df)

swp = USDLiborSwap("6M", rh.referenceDate, date(2009, 9, 22), .04, FixedPayer)
swp2 = EuriborSwap("6M", rh.referenceDate, date(2009, 9, 22), .04, FixedPayer)
