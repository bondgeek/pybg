# setup.py
# bgpy
#

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize

from setup_config import *

extension_paths  = [
 ('bgpy.ql', ['bgpy/ql.pyx', 'bg/date_utilities.cpp']),
 ('bgpy.curves', ['bgpy/curves.pyx']),
 
 ('bgpy.quantlib.currency', ['bgpy/quantlib/currency.pyx']), 
 ('bgpy.quantlib.cashflow', ['bgpy/quantlib/cashflow.pyx']),
 ('bgpy.quantlib.quotes', ['bgpy/quantlib/quotes.pyx']),
 
 ('bgpy.quantlib.math.optimization', ['bgpy/quantlib/math/optimization.pyx']),
 
 ('bgpy.quantlib.time.calendar', ['bgpy/quantlib/time/calendar.pyx']),
 ('bgpy.quantlib.time.date', ['bgpy/quantlib/time/date.pyx']),
 ('bgpy.quantlib.time.daycounter', ['bgpy/quantlib/time/daycounter.pyx']),
 ('bgpy.quantlib.time.schedule', ['bgpy/quantlib/time/schedule.pyx']),
 ('bgpy.quantlib.time.calendars.germany', ['bgpy/quantlib/time/calendars/germany.pyx']),
 ('bgpy.quantlib.time.calendars.null_calendar', ['bgpy/quantlib/time/calendars/null_calendar.pyx']),
 ('bgpy.quantlib.time.calendars.united_kingdom', ['bgpy/quantlib/time/calendars/united_kingdom.pyx']),
 ('bgpy.quantlib.time.calendars.united_states', ['bgpy/quantlib/time/calendars/united_states.pyx']),
 ('bgpy.quantlib.time.daycounters.actual_actual', ['bgpy/quantlib/time/daycounters/actual_actual.pyx']),
 ('bgpy.quantlib.time.daycounters.thirty360', ['bgpy/quantlib/time/daycounters/thirty360.pyx'])

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

setup(name="bgpy",
      ext_modules=extensions,  
      cmdclass={'build_ext': build_ext})
