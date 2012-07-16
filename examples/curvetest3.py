import pybg.ql 
import pybg.curves as curves
from pybg.curvetypes import USDLiborCurve

from alprion.db.curvesdb.history import curves_history

from datetime import date
import logging

ussw = curves_history['USSW']
ussw.keys.sort(reverse=True)


depohdr = ['1D', '1W', '1M', '3M', '6M']
swaphdr = ussw.hdr[7:]

dt0 = date(2012, 7, 11)

pybg.ql.set_eval_date(dt0)

logfile = "curvetest3.log"
logging.basicConfig(level=logging.INFO, filename=logfile)

logging.info("Date: {:%m/%d/%Y}".format(dt0))

print("Number of dates: {}".format(len(ussw.keys)))
print("Log file: {}".format(logfile))

n = 0
for dt in ussw.keys[30:]:
    
    try:    
        depos = dict([(h, .05) for h in depohdr])

        swaps = dict([(h, .05) for h in swaphdr])

        rh = curves.RateHelperCurve(USDLiborCurve("3M"))
        rh.update(depos, {}, swaps, dt)
        
        outstr = "{:3g}){:%m/%d/%Y}: {:.5f}, {:.5f}, {:.5f}"
        print(outstr.format(n,
                            dt,
                            rh.tenorquote("3M"),
                            rh.tenorquote("2Y"),
                            rh.discount(10.)
                            )
              )
    except:
        logging.warning("Error for {:%Y/%m/%d}".format(dt))
        print("Error for {:%Y/%m/%d}".format(dt))
        
        for h in depohdr:
            print h, ussw[dt][h]
            logging.warning("{}, {}".format(h, ussw[dt][h]))
            
        for h in swaphdr:
            print h, ussw[dt][h]
            logging.warning("{}, {}".format(h, ussw[dt][h]))
    
    n += 1