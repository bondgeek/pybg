"""
    Date QDate(Date qdate) {return qdate;}
    
    Date QDate(string datestr)
    {
        boost::smatch matches;
        
        boost::regex yymmdd("(?<Y>[1-9][0-9]{3})(?<M>[0-1][0-9])(?<D>[0-3][0-9])");  
        boost::regex mm_dd_yyyy("(?<M>[0-1][0-9])(-|/)(?<D>[0-3][0-9])(-|/)(?<Y>[1-9][0-9]{3})");
        boost::regex yyyy_mm_dd("(?<D>[0-3][0-9])(-|/)(?<M>[0-1][0-9])(-|/)(?<Y>[1-9][0-9]{3})");
        
        if ( boost::regex_match(datestr, matches, yymmdd) ||
             boost::regex_match(datestr, matches, mm_dd_yyyy) ||
             boost::regex_match(datestr, matches, yyyy_mm_dd)
            ) 
        {
            return Date(atoi( (new string(matches["D"]))->c_str() ), 
                        static_cast<Month>(atoi( (new string(matches["M"]))->c_str() )), 
                        atoi( (new string(matches["Y"]))->c_str() ));        
        }
        
        return Date();
    }

    Date QDate(int datenum)
    {
        int yyyy, dd;
        Month mm;
        
        if (datenum > 19000000) {
            yyyy = datenum / 10000;
            dd = datenum % 100 ;
            mm = static_cast<Month>((datenum % 10000) / 100 );
            return Date(dd, mm, yyyy);
        }
        
        return Date(datenum);
    }
    """

import re

from libcpp.string cimport string
from datetime import date 

cimport pybg.quantlib.time.date as qldate

re_mm = "(?P<M>1[0-2]|0[1-9]|[1-9])"
re_dd = "(?P<D>[0-2][0-9]|3[01]|[1-9])"
re_yyyy = "(?P<Y>[1-9][0-9]{3})"

re_ccyymmdd = re.compile("(?P<Y>[1-9][0-9]{3})(?P<M>[01][0-9])(?P<D>[0-3][0-9])")                                  

re_mm_dd_yyyy = re.compile( "(-|/)".join((re_mm, re_dd, re_yyyy)) )                                         
re_yyyy_mm_dd = re.compile( "(-|/)".join((re_yyyy, re_mm, re_dd)) )


def parse_datetuple(sdate):
    '''Parses ccyymmdd, mm/dd/yyyy or yyyy/mm/dd  
    Returns date tuple: (ccyy, mm, dd) or None if input does not match 
    one of the patterns.
    
    Note: input can be a string or integer (for ccyymmdd pattern).
    
    '''
    sdate = str(sdate)
    
    regex_strings = [re_ccyymmdd, re_mm_dd_yyyy, re_yyyy_mm_dd]
    matches = [re.match(x, sdate) for x in regex_strings]
    
    for m in matches:
        if m:
            return map(int, (m.group('Y'), m.group('M'), m.group('D')))
    
    return None
    
    
def parse_date(sdate):
    '''Parses ccyymmdd, mm/dd/yyyy or yyyy/mm/dd  
    Returns datetime.date or None if input does not match 
    one of the patterns.
    
    Note: input can be a string or integer (for ccyymmdd pattern).

    '''
    tple = parse_datetuple(sdate)
    
    return date(*tple) if tple else None
    

cpdef qldate.Date ql_date(object sdate):
    '''Parses ccyymmdd, mm/dd/yyyy or yyyy/mm/dd  
    Returns QuantLib.Date or None if input does not match 
    one of the patterns.
    
    Note: input can be a string or integer (for ccyymmdd pattern).

    '''
    pydate = parse_date(sdate)
    
    if pydate:
        return qldate.Date.from_datetime(pydate)
    
    return None
    
    