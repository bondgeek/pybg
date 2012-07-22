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

print("Number of dates: {}".format(len(ussw.keys)))
logfile = "curvetest2.log"
with open(logfile, 'w'):
    pass
logging.basicConfig(level=logging.INFO, filename=logfile)

rh = curves.RateHelperCurve(USDLiborCurve("3M"))

n = 0
for dt in ussw.keys:
    
    logging.info("Date: {:%m/%d/%Y}".format(dt))

    try:    
        
        depos = dict([(h, ussw[dt][h]/100.0) for h in depohdr
            if ussw[dt].get(h, None) ])

        swaps = dict([(h, ussw[dt][h]/100.0) for h in swaphdr
            if ussw[dt].get(h, None) ])
        
        
        if not rh.validateNewCurve(depos, {}, swaps):
            print("Building")
            rh = curves.RateHelperCurve(USDLiborCurve("3M"))
            
        rh.update(depos, {}, swaps, dt)
                
        outstr = "{:3g}){} | {}: {:.5f}, {:.5f}, "
        sys.stdout.write(outstr.format(n,
                            dt,
                            rh.curveDate,
                            rh.tenorquote("3M"),
                            rh.tenorquote("10Y")
                            )
                        )
                        
        sys.stdout.write("{}, ".format(rh.referenceDate))
        sys.stdout.write("{:.5f}\n".format(rh.discount(10.)))
        
          
    except:
        logging.info("Error for {:%Y/%m/%d}".format(dt))
        print("Error for {:%Y/%m/%d}".format(dt))
        
        for h in depos:
            print h, ussw[dt][h]
            logging.info("{}, {}".format(h, ussw[dt][h]))
            
        for h in swaps:
            print h, ussw[dt][h]
            logging.info("{}, {}".format(h, ussw[dt][h]))
        
        raise
    
    n += 1