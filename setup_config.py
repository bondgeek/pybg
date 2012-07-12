import os
import sys
import glob

if sys.platform == 'darwin':
    INCLUDE_DIRS = ["/usr/local/include", '.']
    LIBRARY_DIRS = ["/usr/local/lib"]
elif sys.platform == 'win32':
	print("FORMATTING WIN32")
	INCLUDE_DIRS = [r"C:\Program Files (x86)\boost\boost_1_47", r"C:\QuantLib\QuantLib-1.2", '.']
	LIBRARY_DIRS = [r"C:\QuantLib\QuantLib-1.2\lib", r"C:\Program Files (x86)\\boost\boost_1_47\lib"]
	QL_LIBRARY = 'QuantLib'
	
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
    defines = [ ('HAVE_CONFIG_H', None)]
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
        args = ['/GR', '/FD', '/Zm250', '/EHsc' ]
    else:
        args = []

    return args

def get_extra_link_args():
    if sys.platform == 'win32':
        args = ['/subsystem:windows', '/machine:I386']
    else:
        args = []

    return args
 
CYTHON_DIRECTIVES = {"embedsignatur": True}