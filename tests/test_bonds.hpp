/*
 *  test_bonds.hpp
 *  bondgeek
 *
 *  Created by BART MOSLEY on 8/16/12.
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

struct BondTests {
	Date todaysDate;
    
	RateHelperCurve acurve;
    boost::shared_ptr<PricingEngine> discEngine;
	
	BondTests() : 
	todaysDate(TARGET().adjust(Date(20, September, 2004)))
	{
        BOOST_TEST_MESSAGE("\nBond tests");
        string depotenors[] = {"1W", "1M", "3M", "6M", "9M", "1y"};
        double depospots[] = {.055, .055, .055, .055, .055, .055};
        string swaptenors[] = {"2y", "3y", "5y", "10y", "15y", "20y", "30y"};
        double swapspots[] = {.055, .055, .055, .055, .055, .055, .055};
        
        acurve = RateHelperCurve(USDLiborCurve("3M"));
        acurve.update(depotenors, 
                      depospots, 
                      6,
                      swaptenors,                                                                      
                      swapspots,
                      7,
                      todaysDate);
        
        discEngine = \
        createPriceEngine<DiscountingBondEngine>(
                                                 acurve.discountingTermStructure()
                                                 );
		
		
	}
};

BOOST_FIXTURE_TEST_SUITE( bondtest, BondTests )

BOOST_AUTO_TEST_CASE( bond_value )
{
    
    Real coupon = .06;
    Date maturity(15, September,2012);
    Date maturity1(15, September, 2011);
    Date maturity0(15, September, 2010);
    
    Date dated(16,September,2004);
    
    Natural settlementDays = 3;  // Bloomberg OAS1 settle is Oct 19, 2007
    Frequency frequency = Semiannual;
    
    Calendar bondCalendar = UnitedStates(UnitedStates::GovernmentBond);
    DayCounter bondDayCounter = ActualActual(ActualActual::Bond);
    
    Real faceAmount = 100.0; // Notional amount
    Real redemption = 100.0; // Amount paid on redemption
    
    BusinessDayConvention accrualConvention = Unadjusted;
    BusinessDayConvention paymentConvention = Unadjusted;

    SinkingFundBond bnd(coupon,
                        maturity,
                        std::vector<Real>(1, faceAmount),
                        Annual,
                        dated,
                        bondCalendar,
                        settlementDays,
                        bondDayCounter,
                        frequency,
                        redemption,
                        faceAmount,
                        accrualConvention,
                        paymentConvention
                        );
    
    SinkingFundBond bnd0(coupon,
                         maturity0,
                         std::vector<Real>(1, faceAmount),
                         Annual,
                         dated,
                         bondCalendar,
                         settlementDays,
                         bondDayCounter,
                         frequency,
                         redemption,
                         faceAmount,
                         accrualConvention,
                         paymentConvention
                         );
    
    SinkingFundBond bnd1(coupon,
                         maturity1,
                         std::vector<Real>(1, faceAmount),
                         Annual,
                         dated,
                         bondCalendar,
                         settlementDays,
                         bondDayCounter,
                         frequency,
                         redemption,
                         faceAmount,
                         accrualConvention,
                         paymentConvention
                         );
    
    /* Sinkfing fund amortization:
     
     notionals start at par on the dated date and end at zero on maturity.
     redemptions are the prices redemption prices (pct of par, e.g. 101.)
     
     */
    
    double sf_sch[] = {
        40000, 40000, 40000
    };
    int sf_num = 3;
    Frequency sf_freq = Annual;
    
    std::vector<double> sf_bal;
    sf_bal.assign(sf_sch, sf_sch+sf_num);
    
    SinkingFundBond bnd2(coupon,
                         maturity,
                         sf_bal,
                         sf_freq,
                         dated,
                         bondCalendar,
                         settlementDays,
                         bondDayCounter,
                         frequency,
                         redemption,
                         faceAmount,
                         accrualConvention,
                         paymentConvention
                         ); 
    
    bnd0.setPricingEngine(discEngine);
    bnd1.setPricingEngine(discEngine);
    bnd.setPricingEngine(discEngine);
    bnd2.setPricingEngine(discEngine);
    
    double prc0 = bnd0.cleanPrice();
    double prc1 = bnd1.cleanPrice();
    double prc = bnd.cleanPrice();
    double prc2 = bnd2.cleanPrice();
    
    double avgpx = (prc0+prc1+prc)/3.;
    
    BOOST_TEST_MESSAGE("Check average life pricing");
    BOOST_CHECK( abs(prc2-avgpx) < .0001 );	 

	
    
}

BOOST_AUTO_TEST_SUITE_END()