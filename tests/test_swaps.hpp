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
	RateHelperCurve acurve;
    RateHelperCurve *bcurve;
    RateHelperCurve *ccurve;
	
	SwapTests() : 
	calendar(TARGET()), 
	todaysDate(TARGET().adjust(Date(20, September, 2004)))
	{
		Settings::instance().evaluationDate() = todaysDate;
		BOOST_TEST_MESSAGE("\nSwap tests");
		
		string depotenors[] = {"1W", "1M", "3M", "6M", "9M", "1y"};
		double depospots[] = {.0382, 0.0372, 0.0363, 0.0353, 0.0348, 0.0345};
		string swaptenors[] = {"2y", "3y", "5y", "10Y", "15Y"};
		double swapspots[] = {0.037125, 0.0398, 0.0443, 0.05165, 0.055175};
		
		acurve = CurveFactory::instance().ratehelperCurve("EURANN_6M", 
                                                          todaysDate, 
                                                          depotenors, depospots, 6,
                                                          swaptenors, swapspots, 5);
        
        bcurve = new RateHelperCurve(USDLiborCurve("3M"));
        bcurve->update(todaysDate, 
                       depotenors, depospots, 6,
                       swaptenors, swapspots, 5);
        
        ccurve = new RateHelperCurve(USDLiborCurve("3M"));
        ccurve->update(Date(21, January, 2012), 
                       depotenors, depospots, 6,
                       swaptenors, swapspots, 5);
        
        
        Settings::instance().evaluationDate() = todaysDate;
		
	}
};

BOOST_FIXTURE_TEST_SUITE( curvetest, SwapTests )


BOOST_AUTO_TEST_CASE( evaluation_date )
{			
	BOOST_TEST_MESSAGE(">>date: " << Settings::instance().evaluationDate() );
	BOOST_CHECK( Settings::instance().evaluationDate() == todaysDate );
}


BOOST_AUTO_TEST_CASE( curve_values )
{			
    BOOST_TEST_MESSAGE(">>EUR curve.discount(10Y): " << acurve.discount(10.0) );
    BOOST_TEST_MESSAGE(">>USD curve.discount(10Y): " << bcurve->discount(10.0) );
    BOOST_CHECK(  acurve.tenorquote("10Y") ==  0.05165 );
    BOOST_CHECK(  bcurve->tenorquote("10Y") == 0.05165 );
    
}

BOOST_AUTO_TEST_CASE( curve_different_date )
{	
    Date today0 = Settings::instance().evaluationDate();
    
    BOOST_TEST_MESSAGE("\nCheck under different date:" );
    Settings::instance().evaluationDate() = ccurve->curvedate();
    BOOST_TEST_MESSAGE(">>date: " << Settings::instance().evaluationDate() );
	
    BOOST_TEST_MESSAGE(">>USD curve.discount(10Y): " << ccurve->discount(10.0) );
    BOOST_CHECK(  ccurve->tenorquote("10Y") ==  0.05165 );
    
    
    Settings::instance().evaluationDate() = today0;
    BOOST_CHECK( Settings::instance().evaluationDate() == todaysDate );
}

BOOST_AUTO_TEST_CASE( swap_value )
{
	boost::shared_ptr<PricingEngine> swapEngine = createPriceEngine<DiscountingSwapEngine>(
																						   acurve.discountingTermStructure()
																						   );
	
	Real fixedRate = .04;
    Date settlementDate = acurve.settlementDate(); 
    Integer lengthInYears = 5;
    Date maturity = acurve.settlementDate() + lengthInYears*Years;
	
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
