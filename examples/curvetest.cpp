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
              UnitedKingdom(UnitedKingdom::Exchange),
              2,
              Actual360(),
              Semiannual,
              ModifiedFollowing,
              Thirty360(Thirty360::European),
              ActualActual(ActualActual::ISDA)
              )
    {}
    
    TestLiborCurve(string tenor, Frequency fixedFrequency=Semiannual):
    CurveBase(boost::shared_ptr<IborIndex>(new USDLibor( Tenor(tenor) )),
              UnitedKingdom(UnitedKingdom::Exchange),
              2,
              Actual360(),
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
    crv_usdlibor = new RateHelperCurve(TestLiborCurve("3M"));
    
    Calendar crvFixCal = crv_usdlibor->fixingCalendar();
    cout << "fixing calendar: " << crvFixCal << endl;
    
    cout << "Range of dates " << endl;
    Date testDate = todaysDate;
    for (int i=0; i < 90; i++) 
    {
        cout << testDate << " >> " ;
        testDate = crvFixCal.adjust(testDate, ModifiedFollowing);
        cout << testDate << endl ;
        
        testDate = crvFixCal.advance(testDate, -1, Days, Unadjusted);
    }
        
    cout << "Reset date and run curve tests " << endl;
    testDate = todaysDate;
    for (int i=0; i < 90; i++) 
    {
        cout << testDate << " >> " ;
        testDate = crvFixCal.adjust(testDate, ModifiedFollowing);
        cout << testDate << " | " ;
        
        crv_usdlibor->update(testDate, 
                             depotenors, depospots, 6,
                             swaptenors, swapspots, 5);	
        
        Date   refDate = crv_usdlibor->referenceDate();
        cout << "curve ref date: " << refDate << " | "; 
        
        
        double df = crv_usdlibor->discount(10.0);
        cout << "discount(10.): " << df << endl;
        
        testDate = crvFixCal.advance(testDate, -1, Days, Unadjusted);
    }
    
}