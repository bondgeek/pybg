
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


/* TODO: Instrument interface
 - swaps: euro swaps, usd
 - bonds:  UST, corp, tax-exempt munis
 */
int main () 
{
    cout << "QuantLib Version #: " << QL_VERSION << endl ;    
        
    Calendar calendar = TARGET();
//    Date todaysDate = TARGET().adjust( Date(20, September, 2004) );
    Date todaysDate = TARGET().adjust( Date(28, May, 2012) );
    
    Settings::instance().evaluationDate() = todaysDate;
    
    cout << "\n\nToday: " << todaysDate << endl;
    
    
    string futtenors[] = {"ED1", "ED2", "ED3", "ED4", "ED5", "ED6", "ED7", "ED8"};
    double futspots[] = {96.2875, 96.7875, 96.9875, 96.6875, 96.4875, 96.3875, 96.2875, 96.0875};
    
    string depotenors[] = {"1W", "1M", "3M", "6M", "9M", "1y"};
    double depospots[] = {.0382, 0.0372, 0.0363, 0.0353, 0.0348, 0.0345};
    string swaptenors[] = {"2y", "3y", "5y", "10Y", "15Y"};
    double swapspots[] = {0.037125, 0.0398, 0.0443, 0.05165, 0.055175};
    
    cout << "test sc1" << endl;
    RateHelperCurve acurve = CurveFactory::instance().ratehelperCurve("EURANN_6M", 
                                                                      todaysDate, 
                                                                      depotenors, depospots, 6, 
                                                                      swaptenors, swapspots, 5);
    
    cout << "quote:  " << acurve.tenorquote("10Y") << endl;
    
    cout << "test tenors\n" << Period(6, Months) << " | " << Tenor("6M") << endl;
    cout << "fixing calendar: " << acurve.fixingCalendar() << endl;
    
    /*********************
     * SWAPS TO BE PRICED *
     **********************/

    RelinkableHandle<YieldTermStructure>forecastingTermStructure = acurve.forecastingTermStructure();
    
    // constant nominal 1,000,000 Euro
    Real nominal = 1000000.0;
    // fixed leg
    Frequency fixedLegFrequency = Annual;
    BusinessDayConvention fixedLegConvention = Unadjusted;
    BusinessDayConvention floatingLegConvention = ModifiedFollowing;
    DayCounter fixedLegDayCounter = Thirty360(Thirty360::European);
    Rate fixedRate = 0.04;
    DayCounter floatingLegDayCounter = Actual360();
    
    // floating leg
    Frequency floatingLegFrequency = Semiannual;
    
    boost::shared_ptr<IborIndex> euriborIndex(new Euribor6M(forecastingTermStructure));
    
    Spread spread = 0.0;
    
    Integer lenghtInYears = 5;
    VanillaSwap::Type swapType = VanillaSwap::Payer;
    
    
    Date settlementDate = acurve.referenceDate(); 
    Date maturity = acurve.referenceDate() + lenghtInYears*Years;
    Schedule fixedSchedule(settlementDate, maturity,
                           Period(fixedLegFrequency),
                           calendar, fixedLegConvention,
                           fixedLegConvention,
                           DateGeneration::Forward, false);
    Schedule floatSchedule(settlementDate, maturity,
                           Period(floatingLegFrequency),
                           calendar, floatingLegConvention,
                           floatingLegConvention,
                           DateGeneration::Forward, false);
    VanillaSwap spot5YearSwap(swapType, nominal,
                              fixedSchedule, fixedRate, fixedLegDayCounter,
                              floatSchedule, euriborIndex, spread,
                              floatingLegDayCounter);
    
    Date fwdStart = calendar.advance(settlementDate, 1, Years);
    Date fwdMaturity = fwdStart + lenghtInYears*Years;
    Schedule fwdFixedSchedule(fwdStart, fwdMaturity,
                              Period(fixedLegFrequency),
                              calendar, fixedLegConvention,
                              fixedLegConvention,
                              DateGeneration::Forward, false);
    Schedule fwdFloatSchedule(fwdStart, fwdMaturity,
                              Period(floatingLegFrequency),
                              calendar, floatingLegConvention,
                              floatingLegConvention,
                              DateGeneration::Forward, false);
    VanillaSwap oneYearForward5YearSwap(swapType, nominal,
                                        fwdFixedSchedule, fixedRate, fixedLegDayCounter,
                                        fwdFloatSchedule, euriborIndex, spread,
                                        floatingLegDayCounter);
    
    cout << "swap." << endl;
    
    boost::shared_ptr<PricingEngine> swapEngine = createPriceEngine<DiscountingSwapEngine>(
                                                                                           acurve.discountingTermStructure()
                                                                                           );
    
    spot5YearSwap.setPricingEngine(swapEngine);
    oneYearForward5YearSwap.setPricingEngine(swapEngine);
    
    Real NPV;
    Rate fairRate;
    Spread fairSpread;
    
    cout << "spot " << endl;
    NPV = spot5YearSwap.NPV();
    fairSpread = spot5YearSwap.fairSpread();
    fairRate = spot5YearSwap.fairRate();
    
    cout << std::setprecision(2) << std::setw(12) << std::fixed <<
    "NPV : " << NPV << 
    "   | Fair Spread: " << io::rate(fairSpread) << 
    "   | Fair Rate: " << io::rate(fairRate) << endl;
    
    cout << std::setprecision(2) << std::setw(12) << std::fixed <<
    "fx NPV : " << spot5YearSwap.fixedLegNPV() << 
    "   | fx NPV : " << spot5YearSwap.floatingLegNPV() << endl; 
    
    cout << "forward " << endl;
    NPV = oneYearForward5YearSwap.NPV();
    fairSpread = oneYearForward5YearSwap.fairSpread();
    fairRate = oneYearForward5YearSwap.fairRate();
    
    cout << std::setprecision(2) << std::setw(12) << std::fixed <<
    "NPV : " << NPV << 
    "   | Fair Spread: " << io::rate(fairSpread) << 
    "   | Fair Rate: " << io::rate(fairRate) << endl;
    
    cout << std::setprecision(2) << std::setw(12) << std::fixed <<
    "fx NPV : " << oneYearForward5YearSwap.fixedLegNPV() << 
    "   | fx NPV : " << oneYearForward5YearSwap.floatingLegNPV() << endl; 
    
    cout << "test libor clone" << endl;
    RelinkableHandle<YieldTermStructure> indexTermStructure;
    boost::shared_ptr<IborIndex> libor3m(new USDLibor(Period(3, Months), 
                                                      indexTermStructure));
    
    Handle<YieldTermStructure>testTS = acurve.forecastingTermStructure();
    boost::shared_ptr<IborIndex> newlib = libor3m->clone(testTS);
    
    // If one wanted a USD Libor index....
    USDLiborBase testIndex;
    
    cout << "\n\nSwap to compare to first swap " << endl;
    cout << "Qswap" << endl;
    cout << "mty: " << maturity << " | cpn: " << io::rate(fixedRate) << endl;
    EuriborBase euribor(6, Months);
    
    FixedFloatSwap qswp(settlementDate,
                        maturity,
                        fixedRate,
                        euribor(acurve.yieldTermStructurePtr()),
                        FixedPayer,
                        0.0,
                        1000000.0,
                        Annual,
                        Thirty360(Thirty360::European),
                        Unadjusted,
                        Semiannual,
                        Actual360(),
                        ModifiedFollowing,
                        TARGET()
                        );
    
    qswp.setPricingEngine(swapEngine);
    
    cout << std::setprecision(2) << std::setw(12) << std::fixed <<
    "NPV : " << qswp.NPV() << 
    "   | Fair Spread: " << io::rate(qswp.fairSpread()) << 
    "   | Fair Rate: " << io::rate(qswp.fairRate()) << endl; 
    
    cout << std::setprecision(2) << std::setw(12) << std::fixed <<
    "fx NPV : " << qswp.fixedLegNPV() << 
    "   | fl NPV : " << qswp.floatingLegNPV() << endl; 
    
    cout << "Inspect Legs" << endl << endl;
    Leg fixedLeg = qswp.fixedLeg();
    Leg floatingLeg = qswp.floatingLeg();
    
    cout << "Fixed: " << endl;
    Leg::iterator fxIt;
    int cfCount =0;
    Date cfDate;
    double cfAmt;
    double cfDF;
    double cfNPV = 0.0;
    for (fxIt=fixedLeg.begin(); fxIt < fixedLeg.end(); fxIt++) {
        cfDate = (*fxIt)->date();
        cfAmt = (*fxIt)->amount();
        cfDF = acurve.discount((*fxIt)->date());
        cfNPV += cfAmt*cfDF;
        
        cout << cfCount++ << ") " 
        << std::setw(24) << cfDate << " | "  << std::setw(12) 
        << cfAmt << " | " 
        << std::setprecision(6) << cfDF << " | " 
        << std::setprecision(2) << cfNPV
        << endl;
    }

    cout << "Floating: " << endl;
    Leg::iterator flIt;
    cfCount =0;
    cfNPV = 0.0;
    for (flIt=floatingLeg.begin(); flIt < floatingLeg.end(); flIt++) {
        cfDate = (*flIt)->date();
        cfAmt = (*flIt)->amount();
        cfDF = acurve.discount((*flIt)->date());
        cfNPV += cfAmt*cfDF;
        
        cout << cfCount++ << ") " 
        << std::setw(24) << cfDate << " | "  << std::setw(12) 
        << cfAmt << " | " 
        << std::setprecision(6) << cfDF << " | " 
        << std::setprecision(2) << cfNPV
        << endl;
    }
    
    cout << "swp2 " << endl;
    
    SwapType<Euribor> euriborswaps(Annual,
                                   Thirty360(Thirty360::European),
                                   Unadjusted,
                                   Semiannual,
                                   Actual360(),
                                   ModifiedFollowing,
                                   TARGET()
                                   );
    
    cout << "create" << endl;
    boost::shared_ptr<FixedFloatSwap> qswp2 = euriborswaps.create(settlementDate,
                                                                  maturity,
                                                                  fixedRate);
    
    cout << "pricing" << endl;
    qswp2->setEngine(acurve);
    
    cout << "link" << endl;
    euriborswaps.linkIndex(acurve);
    
    cout << std::setprecision(2) << std::setw(12) << std::fixed <<
    "NPV : " << qswp2->NPV() << 
    "   | Fair Spread: " << io::rate(qswp2->fairSpread()) << 
    "   | Fair Rate: " << io::rate(qswp2->fairRate()) << endl; 
    
    cout << std::setprecision(2) << std::setw(12) << std::fixed <<
    "fx NPV : " << qswp2->fixedLegNPV() << 
    "   | fx NPV : " << qswp2->floatingLegNPV() << endl; 
    
    cout << "\nSwap 3: forward" << endl;
    boost::shared_ptr<FixedFloatSwap> qswp3 = euriborswaps.create(fwdStart,
                                                                  fwdMaturity,
                                                                  fixedRate);
    cout << "fixed rate: " << qswp3->fixedRate() << endl;
    qswp3->setPricingEngine(swapEngine);
    euriborswaps.linkIndexTo(acurve.yieldTermStructurePtr());
    
    cout << std::setprecision(2) << std::setw(12) << std::fixed <<
    "NPV : " << qswp3->NPV() << 
    "   | Fair Spread: " << io::rate(qswp3->fairSpread()) << 
    "   | Fair Rate: " << io::rate(qswp3->fairRate()) << endl; 
    
    cout << std::setprecision(2) << std::setw(12) << std::fixed <<
    "fx NPV : " << qswp3->fixedLegNPV() << 
    "   | fx NPV : " << qswp3->floatingLegNPV() << endl; 
    
    cout << "\n\nBonds" << endl;
    
    boost::shared_ptr<BulletBond> bond1(new BulletBond(.045, 
                                                       Date(15, May, 2017), 
                                                       Date(15, May, 2003))
                                        );
    
    cout << "mty: " << bond1->maturityDate() << endl;
    cout << "stl: " << bond1->settlementDate() << endl;
    
    cout << "test" << endl;
    bond1->setEngine(acurve);
    
    cout << "bondprice: " ;
    double prc = bond1->cleanPrice();
    cout << std::setprecision(3) << prc << endl;
    
    cout << "Yield: " ;
    double yld = bond1->yield(prc, bond1->dayCounter(), Compounded, bond1->frequency());
    cout << io::rate(yld) << endl;
    
    cout << "\n\nIMM Stuff\n";
    cout << "settle: " << settlementDate << endl;
    Date  imm = IMM::nextDate(settlementDate);
    string immcode = IMM::code(imm);
    
    Date imm2 = imm_nextDate(imm);
    string immcode2 = imm_nextCode(immcode);
    
    cout << "date: " << imm << " | code: " << immcode << endl; 
    cout << "date: " << imm2 << " | code: " << immcode2 << endl; 
    
    cout << "ED3: " << FuturesTenor("ED3") << endl;
    
    CurveMap depocurve;
    CurveMap futscurve;
    CurveMap swapcurve;
    RateHelperCurve rhcurve( EURiborCurve("6M", Annual) );
    
    if (futscurve.empty()) {
        cout << "futscurve empty " << endl;
    } else {
        cout << "futscurve not empty: " << endl;
    }

    
    for (int i=0; i<2; i++) 
        depocurve[depotenors[i]] = depospots[i];
    
    for (int i=0; i<6; i++) 
        futscurve[futtenors[i]] = futspots[i];
    
    for (int i=0; i<5; i++) 
        swapcurve[swaptenors[i]] = swapspots[i];
    
    rhcurve.update(depocurve, futscurve, swapcurve);
    
    cout << "\n10Y: " << io::rate(rhcurve.tenorquote("10Y")) << endl;
    cout << "discount: " << rhcurve.discount(10.0) << endl;
    return 0;
}
