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

// for arrays
#define LENGTH(x) (sizeof(x)/sizeof(x[0]))


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
		
		Settings::instance().evaluationDate() = todaysDate;
	}
};

BOOST_FIXTURE_TEST_SUITE( bondtest, BondTests )

BOOST_AUTO_TEST_CASE( bulletbond_price_yield_consistency )
{
    BOOST_MESSAGE("Testing consistency of bond price-yield calcs...");
    Rate coupon[] = {.06, .05, .04};
    Date maturity[] = {
        Date(15, September,2012),
        Date(1, November,2015),
        Date(30, June,2011),
        Date(28, February, 2017)
    };
    Date dated[] = {
        Date(15, September,2004),
        Date(1, May,2004),
        Date(30, June,2004),
        Date(1, February, 2004)
    };
    Calendar bondCalendar[] = {
        UnitedStates(UnitedStates::GovernmentBond),
        UnitedStates(UnitedStates::Settlement),
        UnitedStates(UnitedStates::NYSE)
    };
    DayCounter bondDayCounter[] = {
        ActualActual(ActualActual::Bond),
        ActualActual(ActualActual::Actual365),
        ActualActual(ActualActual::ISDA),
        ActualActual(ActualActual::Euro),
        Thirty360(Thirty360::BondBasis),
        Thirty360(Thirty360::USA),
        Thirty360(Thirty360::European),
        Thirty360(Thirty360::EurobondBasis)
    };
    
    Natural settlementDays = 3;
    Frequency frequency = Semiannual;
    
    Real faceAmount = 100.0; // Notional amount
    Real redemption = 100.0; // Amount paid on redemption
    
    BusinessDayConvention accrualConvention = Unadjusted;
    BusinessDayConvention paymentConvention = Unadjusted;
    
    Real tolerance = 1.0e-5;
    
    double prc;
    double yld;
    for (int i=0; i < LENGTH(coupon); i++) {
        for (int j=0; j < LENGTH(maturity); j++) {
            for (int k=0; k < LENGTH(bondCalendar); k++) {
                for (int l=0; l < LENGTH(bondDayCounter); l++) {
                    BulletBond bnd(
                                   coupon[i],
                                   maturity[j],
                                   dated[j],
                                   bondCalendar[k],
                                   settlementDays,
                                   bondDayCounter[l],
                                   frequency,
                                   redemption,
                                   faceAmount,
                                   accrualConvention,
                                   paymentConvention
                                   );
                    
                    bnd.setEngine(acurve);
                    
                    prc = bnd.toPrice();
                    yld = bnd.toYield(prc);
                    
                    BOOST_CHECK( abs(bnd.toPrice(yld) - prc) < tolerance );
                }
            }
        }
    }
     
    BOOST_TEST_MESSAGE("OK");
}

BOOST_AUTO_TEST_CASE( callbond_price_yield_consistency )
{
    BOOST_MESSAGE("Testing consistency of bond price-yield calcs...");
    Rate coupon = .05;
    Date maturity(15, September,2016);
    Date dated(15, September, 2004);
    Date calldate[] = {
        Date(15, September,2011),
        Date(15, September,2012),
        Date(15, September,2013)
    };
    Real callprc[] = {100., 101., 102.};
    Date pardate[] = {
        maturity,
        Date(15, September,2013),
        Date(15, September,2015)
    };
    
    Calendar bondCalendar = UnitedStates(UnitedStates::Settlement);
    DayCounter bondDayCounter = Thirty360(Thirty360::BondBasis);
    
    Natural settlementDays = 3;
    Frequency frequency = Semiannual;
    
    Real faceAmount = 100.0; // Notional amount
    Real redemption = 100.0; // Amount paid on redemption
    
    BusinessDayConvention accrualConvention = Unadjusted;
    BusinessDayConvention paymentConvention = Unadjusted;
    
    Real tolerance = 1.0e-5;
    
    double prc;
    double yld;
    for (int i=0; i < LENGTH(calldate); i++) {

        CallBond bnd(
                     coupon,
                     maturity,
                     calldate[i],
                     callprc[i],
                     pardate[i],
                     dated,
                     bondCalendar,
                     settlementDays,
                     bondDayCounter,
                     frequency,
                     Annual,
                     redemption,
                     faceAmount,
                     accrualConvention,
                     paymentConvention
                     );
        
        bnd.setEngine(acurve);
        
        prc = bnd.toPrice();
        yld = bnd.toYield(prc);
        
        BOOST_CHECK( abs(bnd.toPrice(yld) - prc) < tolerance );
    }
    
    BOOST_TEST_MESSAGE("OK");
}

BOOST_AUTO_TEST_CASE( bond_value_sinker_consistency )
{
    BOOST_MESSAGE("Testing consistency of sinking fund bond calcs...");
    Real coupon = .06;
    Date maturity(15, September,2012);
    Date maturity1(15, September, 2011);
    Date maturity0(15, September, 2010);
    
    Date dated(16,September,2004);
    
    Natural settlementDays = 3;  
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
    
    bnd.setEngine(acurve);

    bnd0.setPricingEngine(discEngine);
    bnd1.setPricingEngine(discEngine);
    bnd2.setPricingEngine(discEngine);
    
    double prc = bnd.toPrice();

    double prc0 = bnd0.cleanPrice();
    double prc1 = bnd1.cleanPrice();
    double prc2 = bnd2.cleanPrice();
    
    double avgpx = (prc0+prc1+prc)/3.;
    
    BOOST_CHECK( abs(prc2-avgpx) < .0001 );	 
    BOOST_TEST_MESSAGE("OK");
}

BOOST_AUTO_TEST_SUITE_END()