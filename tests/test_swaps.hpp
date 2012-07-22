/*
 *  test_swaps.hpp
 *  bondgeek
 *
 *  Created by BART MOSLEY on 6/3/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <bg/bondgeek.hpp>

using namespace QuantLib;
using namespace bondgeek;
using namespace boost::unit_test;

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {
    Integer sessionId() { return 0; }
}
#endif

struct SwapTests {
	Calendar calendar;
	Date todaysDate;
    
    CurveMap depocurve, depocurve2;
    CurveMap futscurve;
    CurveMap swapcurve;
    
	RateHelperCurve acurve;
    RateHelperCurve *bcurve;
    RateHelperCurve *ccurve;
	
	SwapTests() : 
	calendar(TARGET()), 
	todaysDate(TARGET().adjust(Date(20, September, 2004)))
	{
		Settings::instance().evaluationDate() = todaysDate;
		BOOST_TEST_MESSAGE("\nSwap tests");
		
        string futtenors[] = {"ED1", "ED2", "ED3", "ED4", "ED5", "ED6", "ED7", "ED8"};
        double futspots[] = {96.2875, 96.7875, 96.9875, 96.6875, 96.4875, 96.3875, 96.2875, 96.0875};
		string depotenors[] = {"1W", "1M", "3M", "6M", "9M", "1y"};
		double depospots[] = {.0382, 0.0372, 0.0363, 0.0353, 0.0348, 0.0345};
		string swaptenors[] = {"2y", "3y", "5y", "10Y", "15Y"};
		double swapspots[] = {0.037125, 0.0398, 0.0443, 0.05165, 0.055175};
		
        for (int i=0; i<2; i++) 
            depocurve2[depotenors[i]] = depospots[i];

        for (int i=0; i<6; i++) 
            depocurve[depotenors[i]] = depospots[i];
        
        for (int i=0; i<6; i++) 
            futscurve[futtenors[i]] = futspots[i];
        
        for (int i=0; i<5; i++) 
            swapcurve[swaptenors[i]] = swapspots[i];
        
		acurve = CurveFactory::instance().ratehelperCurve("EURANN_6M", 
                                                          todaysDate, 
                                                          depotenors, depospots, 6,
                                                          swaptenors, swapspots, 5);
        
        bcurve = new RateHelperCurve(USDLiborCurve("3M"));
        bcurve->update(depotenors, depospots, 6,
                       swaptenors, swapspots, 5,
                       todaysDate
                       );
        
        ccurve = new RateHelperCurve(USDLiborCurve("3M"));
        ccurve->update(depotenors, depospots, 6,
                       swaptenors, swapspots, 5,
                       Date(21, January, 2012)
                       );
        
        
        Settings::instance().evaluationDate() = todaysDate;
		
	}
};

BOOST_FIXTURE_TEST_SUITE( curvetest, SwapTests )

BOOST_AUTO_TEST_CASE( swap_value2 )
{

    RateHelperCurve rhcurve( EURiborCurve("6M", Annual) );
    
    BOOST_TEST_MESSAGE("SWAP_VALUE2: BUILD TEST CURVE");
    
    rhcurve.update(depocurve2, futscurve, swapcurve);
    
    BOOST_TEST_MESSAGE("Test equivalence of forecastingTermStructure and discountingTermStructure");
	Real d1 = rhcurve.discountingTermStructure().currentLink()->discount(10.0);
	Real d2 = rhcurve.forecastingTermStructure().currentLink()->discount(10.0);
	Real d3 = rhcurve.discountingTermStructure().currentLink()->discount(0.0);
    BOOST_CHECK(abs(d1-d2) < 0.001);
    BOOST_CHECK(d3 > d1);
	
    
}

BOOST_AUTO_TEST_CASE( swap_value3 )
{
    
    BOOST_TEST_MESSAGE("SWAP_VALUE3: Update Curve");
    
    ccurve->update(depocurve2, futscurve, swapcurve);
    
    BOOST_TEST_MESSAGE("Test equivalence of forecastingTermStructure and discountingTermStructure");
	Real d1 = ccurve->discountingTermStructure().currentLink()->discount(10.0);
	Real d2 = ccurve->forecastingTermStructure().currentLink()->discount(10.0);
	Real d3 = ccurve->discountingTermStructure().currentLink()->discount(0.0);
    BOOST_CHECK(abs(d1-d2) < 0.001);
    BOOST_CHECK(d3 > d1);
	
    
}

BOOST_AUTO_TEST_CASE( evaluation_date )
{			
	BOOST_TEST_MESSAGE(">>date: " << Settings::instance().evaluationDate() );
	BOOST_CHECK( Settings::instance().evaluationDate() == todaysDate );
}


BOOST_AUTO_TEST_CASE( curve_values )
{			
    BOOST_TEST_MESSAGE(">>EUR curve.discount(10Y): " << acurve.discount(10.0) );
    BOOST_TEST_MESSAGE(">>USD curve.discount(10Y): " << bcurve->discount(10.0) );
    
    BOOST_TEST_MESSAGE(">>EUR curve.tenorquote(10Y): " << acurve.tenorquote("10Y") );
    BOOST_TEST_MESSAGE(">>USD curve.tenorquote(10Y): " << bcurve->tenorquote("10Y") );
    
    BOOST_CHECK(  acurve.tenorquote("10Y") ==  0.05165 );
    BOOST_CHECK(  bcurve->tenorquote("10Y") == 0.05165 );
    
}

BOOST_AUTO_TEST_CASE( curve_different_date )
{	
    Date today0 = Settings::instance().evaluationDate();
    
    BOOST_TEST_MESSAGE("\nCheck under different date:" );
    Settings::instance().evaluationDate() = ccurve->curveDate();
    BOOST_TEST_MESSAGE(">>date: " << Settings::instance().evaluationDate() );
	
    BOOST_TEST_MESSAGE(">>USD curve.discount(10Y): " << ccurve->discount(10.0) );
    BOOST_TEST_MESSAGE(">>USD curve.tenorquote(10Y): " << ccurve->tenorquote("10Y") );
    BOOST_CHECK(  ccurve->tenorquote("10Y") ==  0.05165 );
    
    Settings::instance().evaluationDate() = today0;
    BOOST_CHECK( Settings::instance().evaluationDate() == todaysDate );
}

BOOST_AUTO_TEST_CASE( curve_change_quote )
{	
	double df0 = ccurve->discount(10.0);
    BOOST_TEST_MESSAGE(">>USD curve.discount(10Y): " << df0 );
    BOOST_TEST_MESSAGE(">>USD curve.tenorquote(10Y): " << ccurve->tenorquote("10Y") );
    BOOST_CHECK( ccurve->tenorquote("10Y") ==  0.05165 );
    
    ccurve->setTenorQuote("10Y", 0.0520);
    double df1 = ccurve->discount(10.0);
    BOOST_TEST_MESSAGE(">>USD curve.discount(10Y): " << df1 );
    BOOST_TEST_MESSAGE(">>USD curve.tenorquote(10Y): " << ccurve->tenorquote("10Y") );
    BOOST_CHECK( ccurve->tenorquote("10Y") ==  0.052 );
    BOOST_CHECK( df1 < df0);
    
    ccurve->setTenorQuote("10Y", 0.05165);
    double df2 = ccurve->discount(10.0);
    BOOST_TEST_MESSAGE(">>USD curve.discount(10Y): " << df2 );
    BOOST_TEST_MESSAGE(">>USD curve.tenorquote(10Y): " << ccurve->tenorquote("10Y") );
    BOOST_CHECK( ccurve->tenorquote("10Y") ==  0.05165 );
    BOOST_CHECK( abs(df2-df0) < 0.0000001 );
    
}

BOOST_AUTO_TEST_CASE( swap_value )
{
	boost::shared_ptr<PricingEngine> swapEngine = createPriceEngine<DiscountingSwapEngine>(
																						   acurve.discountingTermStructure()
																						   );
	
	Real fixedRate = .04;
    Date settlementDate = acurve.referenceDate(); 
    Integer lengthInYears = 5;
    Date maturity = acurve.referenceDate() + lengthInYears*Years;
	
	SwapType<Euribor> euriborswaps(Annual,
								   Thirty360(Thirty360::European),
								   Unadjusted,
								   Semiannual,
								   Actual360(),
								   ModifiedFollowing,
								   TARGET()
								   );
	
	euriborswaps.linkIndexTo(acurve.yieldTermStructurePtr());
	
	boost::shared_ptr<FixedFloatSwap> qswp2 = euriborswaps.create(settlementDate,
																  maturity,
																  fixedRate);
	
	qswp2->setPricingEngine(swapEngine);
    
	BOOST_TEST_MESSAGE("Test equivalence of forecastingTermStructure and discountingTermStructure");
	Real d1 = acurve.discountingTermStructure().currentLink()->discount(10.0);
	Real d2 = acurve.forecastingTermStructure().currentLink()->discount(10.0);
	BOOST_CHECK(abs(d1-d2) < 0.001);
	
	Real p1 = qswp2->NPV();
	BOOST_TEST_MESSAGE("Check Pricing against known value");
	BOOST_CHECK( abs(19065.88-p1) < .001 );	
	
	qswp2->setEngine(acurve);
	Real p2 = qswp2->NPV();
	BOOST_TEST_MESSAGE("Check setEngine pricing");
	BOOST_CHECK( abs(p2-p1) < .0001 );	
	
}


BOOST_AUTO_TEST_SUITE_END()
