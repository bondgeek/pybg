import pybg.ql 
import pybg.curves as curves
from pybg.curvetypes import USDLiborCurve

from pybg.quantlib.time.calendars.united_kingdom import UnitedKingdom, SETTLEMENT
from pybg.quantlib.time.calendar import TARGET 
from pybg.quantlib.time.date import Date

from alprion.db.curvesdb.history import curves_history

from datetime import date
import logging
import sys 

ussw = curves_history['USSW']
ussw.keys.sort(reverse=True)


depohdr = ['1D', '1W', '1M', '3M', '6M']
swaphdr = ussw.hdr[7:]

dt0 = date(2012, 7, 11)

pybg.ql.set_eval_date(dt0)

print("Number of dates: {}".format(len(ussw.keys)))
logging.basicConfig(level=logging.INFO, filename="curvetest2.log")

logging.info("Date: {:%m/%d/%Y}".format(dt0))

uk_calendar = UnitedKingdom(SETTLEMENT)

n = 0
for dt in ussw.keys[:50]:
    
    dt1 = uk_calendar.adjust(Date.from_datetime(dt))
    
    try:    
        depos = dict([(h, ussw[dt][h]/100.0) for h in depohdr
            if ussw[dt].get(h, None) ])

        swaps = dict([(h, ussw[dt][h]/100.0) for h in swaphdr
            if ussw[dt].get(h, None) ])

        rh = curves.RateHelperCurve(USDLiborCurve("3M"))
        rh.update(depos, {}, swaps, dt1)
        
        outstr = "{:3g}){} | {}: {:.5f}, {:.5f}, "
        sys.stdout.write(outstr.format(n,
                            dt,
                            dt1,
                            rh.tenorquote("3M"),
                            rh.tenorquote("2Y")
                            )
                        )
                        
        sys.stdout.write("{}".format(rh.referenceDate))
        sys.stdout.write("{:.5f}\n".format(rh.discount(10.)))
          
    except:
        logging.info("Error for {:%Y/%m/%d}".format(dt))
        print("Error for {:%Y/%m/%d}".format(dt))
        
        for h in depohdr:
            print h, ussw[dt][h]
            logging.info("{}, {}".format(h, ussw[dt][h]))
            
        for h in swaphdr:
            print h, ussw[dt][h]
            logging.info("{}, {}".format(h, ussw[dt][h]))
    
    n += 1