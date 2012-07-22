import pybg.ql 
import pybg.curves as curves
from pybg.curvetypes import USDLiborCurve


from datetime import date

dt0 = date(2012, 7, 11)
depos = {
'12M': 0.010695000000000001,
 '1D': 0.00169,
 '1M': 0.0024875,
 '1W': 0.001996,
 '3M': 0.004561,
 '6M': 0.007344000000000001}

swaps = {'10Y': 0.016415,
 '12Y': 0.018515,
 '15Y': 0.020555,
 '20Y': 0.022275999999999997,
 '25Y': 0.023115,
 '2Y': 0.005003,
 '30Y': 0.02366,
 '3Y': 0.005679999999999999,
 '40Y': 0.023895,
 '4Y': 0.007007,
 '5Y': 0.008685,
 '6Y': 0.010549,
 '7Y': 0.012316,
 '8Y': 0.01387,
 '9Y': 0.015218}


print("\nSetting eval date: %s" % dt0)
pybg.ql.set_eval_date(dt0)

rh = curves.RateHelperCurve(USDLiborCurve("3M"))


rh.update(depos, {}, swaps, dt0)

