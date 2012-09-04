/*
 *  asset_swap.cpp
 *  pybg
 *
 *  Created by BART MOSLEY on 9/2/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#include <bg/bondgeek.hpp>

#include <iostream>

#include <boost/timer.hpp>

using namespace std;
using namespace QuantLib;
using namespace bondgeek;


#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {
    Integer sessionId() { return 0; }
}
#endif


void showAssetSwapCashFlows(AssetSwap &aswap)
{
    Leg fixedLeg = aswap.bondLeg();
    Leg floatLeg = aswap.floatingLeg();
    
    Date cfDate;
    double cfAmt;
    
    cout << endl << "Fixed Leg: " << endl;
    Leg::iterator fxIt;
    for (fxIt=fixedLeg.begin(); fxIt < fixedLeg.end(); fxIt++) 
    {
        cfDate = (*fxIt)->date();
        cfAmt = (*fxIt)->amount();
        cout << cfDate << " | " << cfAmt << endl;
    }
    cout << endl << "Floating Leg: " << endl;
    Leg::iterator flIt;
    for (flIt=floatLeg.begin(); flIt < floatLeg.end(); flIt++) 
    {
        cfDate = (*flIt)->date();
        cfAmt = (*flIt)->amount();
        cout << cfDate << " | " << cfAmt << endl;
    }
    
}

int main (int argc, char * const argv[]) 
{
    
    Date today = Date(31, August, 2012);
    Settings::instance().evaluationDate() = today;
    
    cout << endl << "Today: " << today << endl;
    cout << "Price with a bg curve " << endl;
    
    string depotenors[] = {"1W", "1M", "3M", "6M", "9M"};
    double depospots[] = {0.0018400, 0.0023050, 0.0041825,
        0.0070765, 0.0086780};
    string swaptenors[] = {"2y", "3y", "5y", "10Y", 
        "15Y", "20Y", "30Y"};
    double swapspots[] = {.0040100, 0.0042600, 0.0077300, 0.0106595, 
        0.0211555, 0.02310205, 0.0247007};
    
    cout << "Test with new curve " << endl;
    RateHelperCurve usdLiborCurve = RateHelperCurve(USDLiborCurve("3M"));
    usdLiborCurve.update(depotenors, 
                         depospots, 
                         5,
                         swaptenors,                                                                      
                         swapspots,
                         7,
                         today);
    
    boost::shared_ptr<PricingEngine> discEngine = \
    createPriceEngine<DiscountingBondEngine>(
                                        usdLiborCurve.discountingTermStructure()
                                        );

    // set up the callable bond
    // assuming IL POB 6.63% 2/1/2035, ignoring sinking fund 
    
    Real coupon = .0663;
    Date maturity(1, February, 2035);
    Date dated(1, February, 2010);
    
    Natural settlementDays = 3;  // Bloomberg OAS1 settle is Oct 19, 2007
    Frequency frequency = Semiannual;
    
    Calendar bondCalendar = UnitedStates(UnitedStates::Settlement);
    DayCounter bondDayCounter = Thirty360(Thirty360::BondBasis);
    
    Real faceAmount = 100.0; // Notional amount
    Real redemption = 100.0; // Amount paid on redemption
    
    BusinessDayConvention accrualConvention = Unadjusted;
    BusinessDayConvention paymentConvention = Unadjusted;
    
    BulletBond testbond(coupon, 
               maturity, 
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
    
    testbond.setPricingEngine(discEngine);
    
    cout << "test: " << testbond.cleanPrice() << endl;
    
    cout << "\n\nNow, Asset Swap" << endl;
    // create index and bond   
    
    boost::shared_ptr< Bond > abnd(new BulletBond(coupon, 
                                                  maturity, 
                                                  dated, 
                                                  bondCalendar, 
                                                  settlementDays,
                                                  bondDayCounter,
                                                  frequency,
                                                  redemption,
                                                  faceAmount,
                                                  accrualConvention,
                                                  paymentConvention
                                                  )); 
    
    abnd->setPricingEngine(discEngine);
    
    cout << "Bond: " 
    << "cpn/mty: " << io::rate(coupon) << " due " << maturity << endl
    << "theoretical price: " << abnd->cleanPrice() 
    << "ytm: " << abnd->yield(abnd->cleanPrice(), 
                              bondDayCounter, Compounded, frequency) << endl
    << "accrued: " << abnd->accruedAmount() << endl 
    << endl;
    
    USDLiborBase libor3m("3M");
    boost::shared_ptr<IborIndex> libor3m_ptr = libor3m( \
     usdLiborCurve.yieldTermStructurePtr()
     );
    
    bool payBondCoupon = false;
    Real bondCleanPrice = 112.363 ; 
    Spread spread=0.00; 
    Schedule floatSchedule=Schedule();
    DayCounter floatingDayCount=DayCounter();
    bool parAssetSwap=true;
     
    AssetSwap aswap(payBondCoupon,
                    abnd,
                    bondCleanPrice,
                    libor3m_ptr,
                    spread,
                    floatSchedule,
                    floatingDayCount,
                    parAssetSwap);
    
    boost::shared_ptr<PricingEngine> swapEngine = \
    createPriceEngine<DiscountingSwapEngine>(
                                    usdLiborCurve.discountingTermStructure()
                                    );
    
    aswap.setPricingEngine(swapEngine);
    
    cout << "swap: " 
    << "fair spread: " << io::rate(aswap.fairSpread()) << endl
    << "fair cleanPrice: " <<aswap.fairCleanPrice() << endl
    << endl
    << endl;
    
    showAssetSwapCashFlows(aswap);
    
    return 0;
}
