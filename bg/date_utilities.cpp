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
        cout << " in get_evaluation_date, " ;
        cout << "settings: " << &Settings::instance() << endl;
        return Settings::instance().evaluationDate();
    }
    
    void set_evaluation_date(Date& cob_date) 
    {
        Calendar calendar = TARGET();
        
        // must be a business day
        cob_date = calendar.adjust(cob_date);
        
        Settings::instance().evaluationDate() = cob_date;
    }
}