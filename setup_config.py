import os
import sys
import glob


def collect_ext_dirpaths(_dirpath='bg/quantlib'):
    cython_extension_directories = []
    for dirpath, directories, files in os.walk(_dirpath):
        print('Path: %s' % dirpath)

        # skip the settings package
        if (dirpath.find('settings') > -1 or 
           dirpath.find('test') > -1 or 
           dirpath.find('termstructures') > -1):
            print(">>Skipping %s" % dirpath)
            continue

        # if the directory contains pyx files, cythonise it
        if len(glob.glob('{}/*.pyx'.format(dirpath))) > 0:
            for pyxpath in glob.iglob('{}/*.pyx'.format(dirpath)):
                print("adding: %s" % pyxpath)
                cython_extension_directories.append(pyxpath)

    return cython_extension_directories
	
def get_define_macros():
    defines = [ ('HAVE_CONFIG_H', None) ]
    if sys.platform == 'win32':
        # based on the SWIG wrappers
        defines += [
            (name, None) for name in [
                '__WIN32__', 'WIN32', 'NDEBUG', '_WINDOWS', 'NOMINMAX', 'WINNT',
                '_WINDLL', '_SCL_SECURE_NO_DEPRECATE', '_CRT_SECURE_NO_DEPRECATE',
                '_SCL_SECURE_NO_WARNINGS', 
            ]
        ]
    return defines

def get_extra_compile_args():
    if sys.platform == 'win32':
        args = ['/GR', '/FD', '/Zm250', '/EHsc',
                '/MD', '/Gy', '/O2', '/Oi'
                ]
    else:
        args = []

    return args
    
def get_extra_link_args():
    if sys.platform == 'win32':
        args = ['/subsystem:windows', '/machine:x86']
    else:
        args = []

    return args
 
CYTHON_DIRECTIVES = {"embedsignatur": True}

if sys.platform == 'darwin':
    INCLUDE_DIRS = ["/usr/local/include", '.']
    LIBRARY_DIRS = ["/usr/local/lib"]
    ext_args = dict(
                include_dirs=INCLUDE_DIRS,
                library_dirs=LIBRARY_DIRS,
                libraries = ['QuantLib', 'boost_regex'], # libraries to link
                )
    
elif sys.platform == 'win32':
    INCLUDE_DIRS = [r"C:\QuantLib\QuantLib-1.2",
                    r"C:\Program Files (x86)\boost\boost_1_47", 
                    r"."
                    ]
    LIBRARY_DIRS = [r"C:\QuantLib\QuantLib-1.2\lib",
                    r"C:\Program Files (x86)\boost\boost_1_47\lib"
                    ]
                    
    
    ext_args = dict(
                include_dirs=INCLUDE_DIRS,
                library_dirs=LIBRARY_DIRS,
                define_macros = get_define_macros(),
                extra_compile_args = get_extra_compile_args(),
                extra_link_args = get_extra_link_args(),
                pyrex_directives= CYTHON_DIRECTIVES
                )

extension_paths  = [
 ('pybg.ql', ['pybg/ql.pyx', 'bg/date_utilities.cpp']),
 
 ('pybg.curves', ['pybg/curves.pyx', 
                  'bg/curvebase.cpp',
                  'bg/curves/ratehelpercurve.cpp',
                  'bg/date_utilities.cpp'
                  ]),
                  
 ('pybg.curvetypes.usdliborcurve', ['pybg/curvetypes/usdliborcurve.pyx',
                  'bg/date_utilities.cpp']),
 ('pybg.curvetypes.euriborcurve', ['pybg/curvetypes/euriborcurve.pyx',
                  'bg/date_utilities.cpp']),
 
 ('pybg.indexbases', ['pybg/indexbases.pyx',
                  'bg/date_utilities.cpp']),
  
 ('pybg.instruments.fixedfloatswap', ['pybg/instruments/fixedfloatswap.pyx',
                                      'bg/instruments/fixedfloatswap.cpp', 
                  'bg/curvebase.cpp',
                  'bg/curves/ratehelpercurve.cpp',
                  'bg/date_utilities.cpp']),
 
 ('pybg.instruments.bulletbond', ['pybg/instruments/bulletbond.pyx',
                                  'bg/instruments/bulletbond.cpp', 
                  'bg/curvebase.cpp',
                  'bg/curves/ratehelpercurve.cpp',
                  'bg/date_utilities.cpp']),
                    
 ('pybg.instruments.callbond', ['pybg/instruments/callbond.pyx', 
                                'bg/instruments/callbond.cpp',
                                  'bg/instruments/bulletbond.cpp', 
                  'bg/curvebase.cpp',
                  'bg/curves/ratehelpercurve.cpp',
                  'bg/date_utilities.cpp']),
 
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
