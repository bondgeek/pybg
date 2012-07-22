/*
 *  curvetest.cpp
 *  pybg
 *
 *  Created by BART MOSLEY on 7/17/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */


#include <bg/bondgeek.hpp>

#include <iostream>
#include <algorithm>

#include <boost/timer.hpp>

using namespace std;
using namespace QuantLib;
using namespace bondgeek;

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {
    Integer sessionId() { return 0; }
}
#endif

class TestLiborCurve : public CurveBase {
protected:
public:
    TestLiborCurve():
    CurveBase(boost::shared_ptr<IborIndex>(new USDLibor(Period(3,Months))),
              2,
              Semiannual,
              ModifiedFollowing,
              Thirty360(Thirty360::European),
              ActualActual(ActualActual::ISDA)
              )
    {}
    
    TestLiborCurve(string tenor, Frequency fixedFrequency=Semiannual):
    CurveBase(boost::shared_ptr<IborIndex>(new USDLibor( Tenor(tenor) )),
              2,
              fixedFrequency,
              ModifiedFollowing,
              Thirty360(Thirty360::European),
              ActualActual(ActualActual::ISDA)
              )
    {}
};


int main () 
{
	cout << "QuantLib Version #: " << QL_VERSION << endl ;    
    
    Calendar calendar = TARGET();
    //Date todaysDate = TARGET().adjust( Date(20, September, 2004) );
    Date todaysDate = TARGET().adjust( Date(30, June, 2012) );
	
    Settings::instance().evaluationDate() = todaysDate;
	
    cout << "\n\nToday: " << todaysDate << endl;
    
	string futtenors[] = {"ED1", "ED2", "ED3", "ED4", "ED5", "ED6", "ED7", "ED8"};
	double futspots[] = {96.2875, 96.7875, 96.9875, 96.6875, 96.4875, 96.3875, 96.2875, 96.0875};
    
	string depotenors[] = {"1W", "1M", "3M", "6M", "9M", "1y"};
	double depospots[] = {.0382, 0.0372, 0.0363, 0.0353, 0.0348, 0.0345};
    string swaptenors[] = {"2y", "3y", "5y", "10Y", "15Y"};
    double swapspots[] = {0.037125, 0.0398, 0.0443, 0.05165, 0.055175};
    
    RateHelperCurve *crv_usdlibor;
    crv_usdlibor = new RateHelperCurve(USDLiborCurve("3M"));
        
    crv_usdlibor->update(depotenors, depospots, 6,
                         swaptenors, swapspots, 5,
                         todaysDate
                         );	
    
    cout << "\nRun curve tests " << endl;
    Date testDate = todaysDate;
    for (int i=0; i < 90; i++) 
    {
        cout << testDate << " >> " ;
        Settings::instance().evaluationDate() = testDate;

        cout << "curve ref date: " << crv_usdlibor->referenceDate() << " | "; 
        
        double df = crv_usdlibor->discount(10.0);
        cout << "discount(10.): " << df << " Date OK" << endl;
        
        testDate = crv_usdlibor->advanceCurveDate(-1, Days);
        
    }
    
    cout << "\nReset and \nRun all dates " << endl;
    testDate = todaysDate;
    for (int i=0; i < 90; i++) 
    {
        crv_usdlibor->setCurveDate(testDate);
        cout << testDate << " >> " ;

        cout << crv_usdlibor->curveDate() << " | ";
        cout << "curve ref date: " << crv_usdlibor->referenceDate() << " | "; 
        
        double df = crv_usdlibor->discount(10.0);
        cout << "discount(10.): " << df << " Date OK" << endl ;

        testDate = TARGET().advance(testDate, -1, Days);
        
    }
    
    CurveMap crv_quotes = crv_usdlibor->curveQuotes();
    CurveMap::iterator it;
    cout <<"\n\nCurve " << endl;
    for (it = crv_quotes.begin(); it != crv_quotes.end(); it++)
    {
        cout << (*it).first << ": " << (*it).second << endl;
    }
    
}