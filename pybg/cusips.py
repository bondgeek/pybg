'''
Utilities for handling cusip check digits
'''
# References:
# https://www.cusip.com/static/html/webpage/pdf/CUSIPIntro_%207.26.2007.pdf
# 

__all__ = ['cusipcheckdigit', 'ischeckdigit', 'validate_cusip']
            
import string
import re

# constants
# note: "*@#" are for private placements
# 
_ppnchars =  "*@#"
_cusipchars = "".join((string.digits, string.ascii_uppercase, _ppnchars))
_cusip2value = dict((ch, n) for n,ch in enumerate(_cusipchars))
_cusipweights = [1,2,1,2,1,2,1,2]

_qregexstr = r"""
 ([0-9]{3})                 # first 3 issuer digits, 99[0-9]xxx for user codes
 (([0-9]{2})([0-9A-Z*@#]))  # last 3 issuer digits, xxx99[0-9A-Z] for user codes
 ([0-9A-Z*@#]{2})           # match issue digits
 ((?!.)|[0-9])              # check digit, or '', in group(6) 
 """

qregex = re.compile(_qregexstr , re.VERBOSE)
 
def cusipcheckdigit(cusip):
    '''
    Calculate and return check digit. 
    Assumes validated string--8 or 9 digits with alphas
    
    '''
    cusip = cusip.upper()
    digits = [(w*_cusip2value[ch]) for w,ch in zip(_cusipweights, cusip)]
    cs = sum([(x%10+x//10) for x in digits]) % 10
    
    return str((10-cs)%10)
 
def ischeckdigit(cusip):
    '''
    True if string has nine digits with the ninth being the check digit
    
    '''
    if not isinstance(cusip, str):
        return False
    qsp = cusip.upper()

    qmatch = qregex.match(qsp)
    
    if qmatch:
        checkdig = cusipcheckdigit(qsp)
        return qmatch.group(6) == checkdig
    return False

def validate_cusip(cusip):
    '''
    Return validated full cusip, or None if validation fails
    validated cusip: uppercase alphas, with check digit
    validation:  8 digit core cusip, correct check digit if provided
    '''
    if not isinstance(cusip, str):
        return None
    qsp = cusip.upper()

    qmatch = qregex.match(qsp)
    
    if qmatch:
        checkdig = cusipcheckdigit(qsp)
        if not qmatch.group(6):
            return "".join((qsp, checkdig))
        elif qmatch.group(6) == checkdig:
            return qsp

    return None
