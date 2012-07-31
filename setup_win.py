# setup.py
# pybg
#

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize

from setup_config import *

extension_paths  = [
 #('pybg.ql', ['pybg/ql.pyx', 'bg/date_utilities.cpp']),
 ('pybg.version', ['pybg/version.pyx']),
 #('pybg.quantlib.currency', ['pybg/quantlib/currency.pyx']), 
 #('pybg.quantlib.cashflow', ['pybg/quantlib/cashflow.pyx']),
 ('pybg.quantlib.quotes', ['pybg/quantlib/quotes.pyx']),
 #('pybg.quantlib.time.calendar', ['pybg/quantlib/time/calendar.pyx']),
 #('pybg.quantlib.time.date', ['pybg/quantlib/time/date.pyx']),
 #('pybg.quantlib.time.daycounter', ['pybg/quantlib/time/daycounter.pyx']),
 #('pybg.quantlib.math.optimization', ['pybg/quantlib/math/optimization.pyx']),
 
 ]

print("\nCythonizing...\n")    
collected_extensions = cythonize(
        [Extension(
                extname, # name of extension
                sources=extsources, #  our Cython source
                language="c++",
                **ext_args
                ) # causes Cython to create C++ source
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
