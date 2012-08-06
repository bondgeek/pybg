# setup.py
# pybg
#

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize

from setup_config import *

extension_paths_full  = [
 ('pybg.ql', ['pybg/ql.pyx', 'bg/date_utilities.cpp']),
 ('pybg.version', ['pybg/version.pyx']),
 
 ('pybg.curves', ['pybg/curves.pyx']),
                  
 ('pybg.curvetypes.usdliborcurve', ['pybg/curvetypes/usdliborcurve.pyx']),
 
 ('pybg.curvetypes.euriborcurve', ['pybg/curvetypes/euriborcurve.pyx']),
 
 ('pybg.indexbases', ['pybg/indexbases.pyx']),
  
 ('pybg.instruments.fixedfloatswap', ['pybg/instruments/fixedfloatswap.pyx']),
 
 ('pybg.instruments.bulletbond', ['pybg/instruments/bulletbond.pyx']),
                    
 ('pybg.instruments.callbond', ['pybg/instruments/callbond.pyx']),
 
 ('pybg.quantlib.currency', ['pybg/quantlib/currency.pyx']), 
 ('pybg.quantlib.cashflow', ['pybg/quantlib/cashflow.pyx']),
 ('pybg.quantlib.quotes', ['pybg/quantlib/quotes.pyx']),
 
 ('pybg.quantlib.math.optimization', ['pybg/quantlib/math/optimization.pyx']),
 
 ('pybg.quantlib.time.calendar', ['pybg/quantlib/time/calendar.pyx']),
 ('pybg.quantlib.time.date', ['pybg/quantlib/time/date.pyx']),
 ('pybg.quantlib.time.daycounter', ['pybg/quantlib/time/daycounter.pyx']),
 ('pybg.quantlib.time.schedule', ['pybg/quantlib/time/schedule.pyx']),
 ('pybg.quantlib.time.calendars.germany', ['pybg/quantlib/time/calendars/germany.pyx']),
 ('pybg.quantlib.time.calendars.null_calendar', ['pybg/quantlib/time/calendars/null_calendar.pyx']),
 ('pybg.quantlib.time.calendars.united_kingdom', ['pybg/quantlib/time/calendars/united_kingdom.pyx']),
 ('pybg.quantlib.time.calendars.united_states', ['pybg/quantlib/time/calendars/united_states.pyx']),
 ('pybg.quantlib.time.daycounters.actual_actual', ['pybg/quantlib/time/daycounters/actual_actual.pyx']),
 ('pybg.quantlib.time.daycounters.thirty360', ['pybg/quantlib/time/daycounters/thirty360.pyx']),
 
 ('pybg.quantlib.index', ['pybg/quantlib/index.pyx']),
 ('pybg.quantlib.indexes.euribor', ['pybg/quantlib/indexes/euribor.pyx']),
 ('pybg.quantlib.indexes.ibor_index', ['pybg/quantlib/indexes/ibor_index.pyx']),
 ('pybg.quantlib.indexes.interest_rate_index', [
            'pybg/quantlib/indexes/interest_rate_index.pyx']),
 ('pybg.quantlib.indexes.libor', ['pybg/quantlib/indexes/libor.pyx']),
 
 ('pybg.quantlib.termstructures.volatility.equityfx.black_vol_term_structure', [
            'pybg/quantlib/termstructures/volatility/equityfx/black_vol_term_structure.pyx'
            ]),
 ('pybg.quantlib.termstructures.yields.flat_forward', [
            'pybg/quantlib/termstructures/yields/flat_forward.pyx'
            ]),
            
 ('pybg.quantlib.termstructures.yields.piecewise_yield_curve', [
            'pybg/quantlib/termstructures/yields/piecewise_yield_curve.pyx',
            'pybg/quantlib/termstructures/yields/_piecewise_support_code.cpp'
            ]),
 ('pybg.quantlib.termstructures.yields.yield_term_structure', ['pybg/quantlib/termstructures/yields/yield_term_structure.pyx']),
 ('pybg.quantlib.termstructures.yields.zero_curve', ['pybg/quantlib/termstructures/yields/zero_curve.pyx']),
 ('pybg.quantlib.termstructures.yields.rate_helpers', ['pybg/quantlib/termstructures/yields/rate_helpers.pyx'])

 ]

print("\nCythonizing...\n")    
collected_extensions = cythonize(
        [Extension(
                extname, # name of extension
                sources=extsources, #  our Cython source
                language="c++",
                **ext_args
                ) # causes Cython to create C++ source
        for extname, extsources in extension_paths_win
        ]
        )

extensions = collected_extensions
print("\nAll the extensions are gathered:")
for ext in extensions:
    print ext.name
print("\n\nBeginning\n\n")

setup(name="pybg",
      ext_modules=extensions,  
      cmdclass={'build_ext': build_ext})
