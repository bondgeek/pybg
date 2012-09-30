# -*- coding: utf-8 -*-
"""
Created on 9/29/2012

@author: bart mosley
@license: bg research llc, MIT License

"""

import numpy as np
from scipy.stats import norm

from bgtools.utils.dates import parse_date

def nprime(x):
     return np.exp(-x*x/2.0)/np.sqrt(2.*np.pi)

#
# S, stock price
# X, strike
# T, time in years 
# r, short rate in continous compounding (=log(1+discount))
# v, volatility (sigma)

def bs_d_values(S, X, T, r, v):
    d1 = (np.log(S/X) + (r + v * v/2.) * T) / (v*np.sqrt(T))
    
    d2 = d1 - v * np.sqrt(T)

    return d1, d2
    
def BlackScholes(S, X, T, r, v, putcall='c'):
    d1, d2 = bs_d_values(S, X, T, r, v)
    
    if putcall.upper() == 'C':
        return S*norm.cdf(d1) - X*np.exp(-r*T)*norm.cdf(d2)
        
    else:
        return - S*norm.cdf(-d1) + X*np.exp(-r*T)*norm.cdf(-d2)

def BlackScholesGreeks(S, X, T, r, v):
    '''
    Greeks on a non-dividend paying stock
    
    '''
    
    d1, d2 = bs_d_values(S, X, T, r, v)
    
    gamma = nprime(d1) / (S * v * np.sqrt(T))
    vega = S*np.sqrt(T)*nprime(d1)
    
    call = {
        'value':
            S*norm.cdf(d1) - X*np.exp(-r*T)*norm.cdf(d2),
        'delta':
            norm.cdf(d1),
        'gamma':
            gamma,
        'theta':
            -S*nprime(d1)*v/(2.* np.sqrt(T)) - r*X*np.exp(-r*T)*norm.cdf(d2),
        'vega':
            vega,
        'rho':
            X*T*np.exp(-r*T)*norm.cdf(d2)
        }
    
    put = {
        'value':
            - S*norm.cdf(-d1) + X*np.exp(-r*T)*norm.cdf(-d2),
        'delta':
            norm.cdf(d1) - 1,
        'gamma':
            gamma,
        'theta':
            -S*nprime(d1)*v/(2.* np.sqrt(T)) + r*X*np.exp(-r*T)*norm.cdf(-d2),
        'vega':
            vega,
        'rho':
            -X*T*np.exp(-r*T)*norm.cdf(-d2)
        }
    
    return dict(call=call, put=put)
    
    