/*
 *  date_utilities.hpp
 *  QuantLibFun
 *
 *  Created by BART MOSLEY on 5/16/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 *  Note: daysMinMax & numSubPeriod copied from QuantLib library
 *        AmortizingFixedRateBond classes
 *
 */
#ifndef DATE_UTILITIES_H
#define DATE_UTILITIES_H

#include <ql/quantlib.hpp>

#include <stdlib.h>
#include <ctype.h>

#include <string>
#include <vector>
#include <utility>
#include <ostream>
#include <map>

using namespace std;
using namespace QuantLib;

namespace bondgeek {
    
    // string manipulations functions
    inline std::string &rtrim(std::string &s) {
        string whitespaces (" \t\f\v\n\r");
        size_t found;
        
        found=s.find_last_not_of(whitespaces);
        if (found!=string::npos)
            s.erase(found+1);
        else
            s.clear();            // str is all whitespace
        
        return s;
    }
    inline std::string &ltrim(std::string &s) {
        string whitespaces (" \t\f\v\n\r");
        size_t found;
        
        found=s.find_first_not_of(whitespaces);
        if (found!=string::npos)
            s.erase(0, found);
        else
            s.clear();            // str is all whitespace
        
        return s;
    }
    inline std::string &trim(std::string &s) {
        return rtrim(ltrim(s));
    }

    // Period, Quote interfaces
    std::pair<Integer,Integer> daysMinMax(const Period& p);
    Integer numSubPeriod(const Period& subPeriod,
                         const Period& superPeriod
                         ) ;
    Period Tenor(string);
    int FuturesTenor(string);
    
    // IMM Interfaces
    bool isIMMdate(Date &date_ref, bool mainCycle=true);
    bool isIMMcode(std::string &instring, bool mainCycle=true);
    std::string imm_code(Date &date_ref);
    Date imm_date(std::string immCode, Date ref_date=Date());
    Date imm_nextDate(Date &date_ref, bool mainCycle=true);
    std::string imm_nextCode(std::string immCode, 
                             Date date_ref=Date(),
                             bool mainCycle=true
                             );
    
    // Settings
    Date get_evaluation_date();
    void set_evaluation_date(Date& cob_date);

}
#endif
