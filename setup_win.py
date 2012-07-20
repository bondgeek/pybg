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
