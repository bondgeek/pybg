import pybg.curves as qlc
from datetime import date
depos = dict([(str(n)+"M", .05) for n in [1, 3, 6, 9, 12]])
swaps = dict([(str(n)+"Y", .05) for n in [2, 3, 5, 7, 10, 12, 15, 20, 30]])
rh = qlc.RateHelperCurve(qlc.USDLiborCurve("3M"))
rh.update( depos, swaps, date(2012, 1, 21) )
