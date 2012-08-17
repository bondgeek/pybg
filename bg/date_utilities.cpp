/*
 *  bondgeek.cpp
 *  QuantLibFun
 *
 *  Created by Bart Mosley on 5/11/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 *  Note: daysMinMax & numSubPeriod copied from QuantLib library
 *        AmortizingFixedRateBond classes
 */

#include <bg/date_utilities.hpp>

namespace bondgeek {

    // Tenor & Period manipulations
    std::pair<Integer,Integer> daysMinMax(const Period& p) {
        switch (p.units()) {
            case Days:
                return std::make_pair(p.length(), p.length());
            case Weeks:
                return std::make_pair(7*p.length(), 7*p.length());
            case Months:
                return std::make_pair(28*p.length(), 31*p.length());
            case Years:
                return std::make_pair(365*p.length(), 366*p.length());
            default:
                QL_FAIL("unknown time unit (" << Integer(p.units()) << ")");
        }
    }
    
    Integer numSubPeriod(const Period& subPeriod,
                         const Period& superPeriod
                         ) 
    {
        Integer numSubPeriods;
        
        std::pair<Integer, Integer> superDays(daysMinMax(superPeriod));
        std::pair<Integer, Integer> subDays(daysMinMax(subPeriod));
        
        //obtain the approximate time ratio
        Real minPeriodRatio =
        ((Real)superDays.first)/((Real)subDays.second);
        Real maxPeriodRatio =
        ((Real)superDays.second)/((Real)subDays.first);
        Integer lowRatio = static_cast<Integer>(std::floor(minPeriodRatio));
        Integer highRatio = static_cast<Integer>(std::ceil(maxPeriodRatio));
        
        try {
            for(Integer i=lowRatio; i <= highRatio; ++i) {
                Period testPeriod = subPeriod * i;
                if(testPeriod == superPeriod) {
                    numSubPeriods = i;
                    return numSubPeriods;
                }
            }
        } catch(Error e) {
            return -1;
        }
        
        return 0;
    }
    
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
}