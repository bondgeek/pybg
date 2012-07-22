import pybg.ql 
import pybg.curves as curves
from pybg.curvetypes import USDLiborCurve

from pybg.quantlib.time.calendars.united_kingdom import UnitedKingdom, SETTLEMENT
from pybg.quantlib.time.calendar import TARGET 
from pybg.quantlib.time.date import Date

from alprion.db.curvesdb.history import curves_history

import sys 
from datetime import date
import logging

ussw = curves_history['USSW']
ussw.keys.sort(reverse=True)


depohdr = ['1D', '1W', '1M', '3M', '6M']
swaphdr = ussw.hdr[7:]

dt0 = date(2012, 7, 4)

pybg.ql.set_eval_date(dt0)

logfile = "curvetest3.log"
with open(logfile, 'w'):
    pass
    
logging.basicConfig(level=logging.INFO, filename=logfile)

logging.info("Date: {:%m/%d/%Y}".format(dt0))

print("Number of dates: {}".format(len(ussw.keys)))
print("Log file: {}".format(logfile))


rh = curves.RateHelperCurve(USDLiborCurve("3M"))

n = 0
for dt in ussw.keys:
    
    try:    
        depos = dict([(h, .05) for h in depohdr])

        swaps = dict([(h, .05) for h in swaphdr])

        rh.update(depos, {}, swaps, dt)
        
        outstr = "{:3g}){} | {}: {:.5f}, {:.5f}, "
        sys.stdout.write(outstr.format(n,
                            dt,
                            rh.curveDate,
                            rh.tenorquote("3M"),
                            rh.tenorquote("2Y")
                            )
                        )
        sys.stdout.write("{:.5f}, {}\n".format(rh.discount(10.), rh.referenceDate))
              
    except:
        logging.warning("Error for {:%Y/%m/%d}".format(dt))
        print("Error for {:%Y/%m/%d}".format(dt))
        
        for h in depohdr:
            print h, ussw[dt][h]
            logging.warning("{}, {}".format(h, ussw[dt][h]))
            
        for h in swaphdr:
            print h, ussw[dt][h]
            logging.warning("{}, {}".format(h, ussw[dt][h]))
    
        raise
    n += 1