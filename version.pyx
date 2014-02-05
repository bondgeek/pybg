# distutils: language = c++
# not using distutils for libraries, Visual Studio auto-linking doesn't like

__quantlib_version__ = QL_VERSION
__quantlib_lib_version__ = QL_LIB_VERSION
__quantlib_hex_version__ = QL_HEX_VERSION

def version():
    return __quantlib_version__
 