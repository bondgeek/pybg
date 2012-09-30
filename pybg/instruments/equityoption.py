import pdb

import numpy as np
from scipy.stats import norm

from bgtools.utils.dates import parse_date

from pybg.settings import get_eval_date
from pybg.enums import (
    BusinessDayConventions, Calendars, DayCounters, TimeUnits
    )
    
def nprime(x):
     return np.exp(-x*x/2.0)/np.sqrt(2.*np.pi)

class OptionType:
    CALL = 1
    PUT = -1


class EquityOption(object):
    """
    Equity Option: European exercise
    strike:             strike price 
    expiration:         date of expiration
    dividend:           dividend rate
    putcall:            1 = call, -1 = put (see OptionType class)
    settlementDays:     CBOE standard = 1
    calendar:           default UnitedStates(NYSE)
    
    """
    def __init__(self,
                 strike,
                 expiration,
                 dividend=None,
                 putcall=OptionType.CALL,
                 settlementDays = 1,
                 calendar=Calendars.UnitedStates(Calendars.NYSE)
                 ):
        
        self._strike = float(strike)
        self._expiration = parse_date(expiration)
        self._dividend = dividend if dividend else 0.0
        self._putcall = putcall
        self._calendar = calendar  
        self._settlementDays = settlementDays
        self._settlement = None
        
    @property
    def settlementDate(self):
        dt = getattr(self, "_settlement", None)
        if not dt:
            dt = Calendars.advance(get_eval_date(), 
                                   self.settlementDays,
                                   TimeUnits.Days,
                                   self._calendar, 
                                   BusinessDayConventions.Following)
        self._settlement = dt
        
        return dt
    
    @property 
    def strike(self):
        return self._strike
    
    @property
    def expiration(self):
        return self._expiration
        
    @property
    def dividend(self):
        return self._dividend
            
    @property
    def settlementDays(self):
        return self._settlementDays
    
    @settlementDays.setter
    def settlementDays(self, ndays):
        self._settlementDays = ndays
            
    def calc(self, S, v, r, greeks=True):
        """
        S, stock price
        v, volatility (sigma) 
        r, short rate in continous compounding (=log(1+discount))
        
        """
        d = self._putcall
        X = self.strike
        q = self.dividend
    
        cdf = norm.cdf
        exp = np.exp
        
        T = DayCounters.year_fraction(self.settlementDate, 
                                      self.expiration,
                                      DayCounters.ActualActual())
        
        pv_q = exp(-q * T)
        
        d1 = (np.log(S/X) + (r - q + v * v/2.) * T) / (v*np.sqrt(T))
        d2 = d1 - v * np.sqrt(T)
        
        value = d * (S * pv_q * cdf(d * d1) - X * exp(-r * T) * cdf(d * d2))
        
        if greeks:
            N = nprime(d1)
            sqrtT = np.sqrt(T)
            pvr = exp(-r * T)
            
            gamma = pv_q * N / (S * v * sqrtT)
            vega = S * sqrtT * N * pv_q
            
            if d > 0:
                delta = pv_q * cdf(d1)
                theta = S*pv_q*(q*cdf(d1) - N*v/(2.*sqrtT)) - r*X*pvr*cdf(d2)
                rho = X * T * exp(-r*T) * cdf(d2)
            else:
                delta = pv_q * (cdf(d1) - 1)                
                theta = -S*pv_q*(q*cdf(-d1) + N*v/(2.*sqrtT)) - r*X*pvr*cdf(-d2)
                rho = -X * T * np.exp(-r*T) * cdf(-d2)
                
            self.greeks = {
                'value': value,
                'T': T,
                'delta': delta,
                'gamma': gamma,
                'theta': theta,
                'vega': vega,
                'rho':rho
                }
        
        return value
    