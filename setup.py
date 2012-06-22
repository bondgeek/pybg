# setup.py
# pybg
#

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize

from setup_config import *

extension_paths  = [
 ('pybg.ql', ['pybg/ql.pyx', 'bg/date_utilities.cpp']),
 
 ('pybg.curves', ['pybg/curves.pyx', 
                  'bg/curvebase.cpp',
                  'bg/curves/ratehelpercurve.cpp']),
 
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
 ('pybg.quantlib.time.daycounters.thirty360', ['pybg/quantlib/time/daycounters/thirty360.pyx'])

 ]
 
 
print("\nCythonizing...\n")    
collected_extensions = cythonize(
        [Extension(
                extname, # name of extension
                sources=extsources, #  our Cython source
                include_dirs=INCLUDE_DIRS,
                library_dirs=LIBRARY_DIRS,
                libraries = ['QuantLib', 'boost_regex'], # libraries to link
                language="c++") # causes Cython to create C++ source
        for extname, extsources in extension_paths
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
