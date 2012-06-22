import os
import sys
import glob

if sys.platform == 'darwin':
    INCLUDE_DIRS = ["/usr/local/include", '.']
    LIBRARY_DIRS = ["/usr/local/lib"]


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