import pybg.ql as ql
import pybg.curves as curves
from pybg.curvetypes import USDLiborCurve

from pybg.quantlib.time.calendars.united_kingdom import UnitedKingdom, SETTLEMENT
from pybg.quantlib.time.calendar import TARGET 
from pybg.quantlib.time.date import Date

from pybg.instruments.fixedfloatswap import USDLiborSwap
from pybg.enums import TimeUnits, SwapPayType

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
for dt in ussw.keys[:100]:
    
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
                
        outstr = "{:3g}){} | {}: {:.5f}, "
        sys.stdout.write(outstr.format(n,
                            dt,
                            rh.curveDate,
                            rh.tenorquote("10Y")
                            )
                        )
                        
        sys.stdout.write("{}, ".format(rh.referenceDate))
        sys.stdout.write("{:.5f} | ".format(rh.discount(10.)))
        
        mty = TARGET().advance(ql.qldate_from_pydate(rh.referenceDate), 
                               10, 
                               TimeUnits.Years)
                               
        cpn = rh.tenorquote("10Y")
        swp = USDLiborSwap("3M", 
                           rh.referenceDate, 
                           mty, 
                           cpn, 
                           SwapPayType.FixedPayer)

        swp.setEngine(rh)
        
        outstr = "pay: {:.3f}% mty: {} {:.5f}\n"
        sys.stdout.write(outstr.format(cpn*100., mty, swp.NPV))
        
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