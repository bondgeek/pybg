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
print("Include Dirs: \n%s" % INCLUDE_DIRS)  
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
