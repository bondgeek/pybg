/*
 *  bondgeek.cpp
 *  QuantLibFun
 *
 *  Created by Bart Mosley on 5/11/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include <bg/date_utilities.hpp>

namespace bondgeek {
	
	/*
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
    */

    int FuturesTenor(string _tenorstr) 
    {
        std::string tstr = trim(_tenorstr);
        int n = atoi( tstr.substr( 2, tstr.length() ).c_str() );
        return n;
    }
    
    Period Tenor(string _tenorstr) {
        std::string tstr = trim(_tenorstr);  
        map<char, TimeUnit> tunits;
        tunits['D'] = Days;
        tunits['W'] = Weeks;
        tunits['M'] = Months;
        tunits['Y'] = Years;
        
        char c = toupper( *tstr.substr(tstr.length()-1).c_str() );
        
        if (tunits.count(c)) {
            TimeUnit t = tunits[c];
            int n = atoi( tstr.substr( 0, tstr.length()-1 ).c_str() );
            return Period(n, t);
        }
        
        return Period();
    }
    
    // IMM Interfaces
    bool isIMMdate(Date &date_ref, bool mainCycle) 
    {
        return IMM::isIMMdate(date_ref, mainCycle);
    }
    
    bool isIMMcode(std::string &instring, bool mainCycle)
    {
        return IMM::isIMMcode(instring, mainCycle);
    }
    
    std::string imm_code(Date &date_ref)
    {
        if ( isIMMdate(date_ref) )
            return IMM::code(date_ref);
        else {
            return IMM::code(IMM::nextDate(date_ref));
        }

    }
    
    Date imm_date(std::string immCode, Date ref_date)
    {
        if ( isIMMcode(immCode) ) {
            return IMM::date(immCode, ref_date);
        }
        else {
            return Date();
        }
        
    }
    
    Date imm_nextDate(Date &date_ref, bool mainCycle)
    {
        return IMM::nextDate(date_ref, mainCycle);
    }
    
    std::string imm_nextCode(std::string immCode, 
                             Date date_ref, 
                             bool mainCycle)
    {
        return IMM::nextCode(immCode, mainCycle, date_ref);
    }
    
    // Settings
    Date get_evaluation_date()
    {
        return Settings::instance().evaluationDate();
    }
    
    void set_evaluation_date(Date& cob_date) 
    {
        Calendar calendar = TARGET();
        
        // must be a business day
        cob_date = calendar.adjust(cob_date);
        
        Settings::instance().evaluationDate() = cob_date;
    }
    
	/*
    void set_evaluation_date(int cob_date) 
    {
        Date qdate_ref = QDate(cob_date);
        set_evaluation_date( qdate_ref );
    }

    void set_evaluation_date(string &cob_date) 
    {
        Date qdate_ref = QDate(cob_date);
        set_evaluation_date( qdate_ref );
    }
    */
}