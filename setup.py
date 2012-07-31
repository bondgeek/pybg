# setup.py
# pybg
#

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize

from setup_config import *

print("\nCythonizing...\n")    
collected_extensions = cythonize(
        [Extension(
                extname,                # name of extension
                sources=extsources,     #  our Cython source
                language="c++",         # causes Cython to create C++ source
                **ext_args
                ) 
        for extname, extsources in extension_paths
        ]
    )
        
extensions = collected_extensions
print("\nAll the extensions are gathered:")
for ext in extensions:
    print ext.name
print("\n\nBeginning\n\n")

setup(name="pybg",
      version='0.1.0',
      ext_modules=extensions,  
      packages=['pybg',
                'pybg.instruments',
                'pybg.curvetypes',
                'pybg.quantlib',
                'pybg.quantlib.indexes',
                'pybg.quantlib.math',
                'pybg.quantlib.termstructures',
                'pybg.quantlib.termstructures.volatility',
                'pybg.quantlib.termstructures.yields',
                'pybg.quantlib.time',
                'pybg.quantlib.time.calendars',
                'pybg.quantlib.time.daycounters'
                ],
      cmdclass={'build_ext': build_ext})
