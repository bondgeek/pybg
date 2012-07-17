#!/bin/sh
find pybg -name \*.pxi -exec git add {} \;
find pybg -name \*.pyx -exec git add {} \;
find pybg -name \*.pxd -exec git add {} \;
find pybg -name \*.py -exec git add {} \;

find bg -name \*.*pp -exec git add {} \;

find tests -name \*.*pp -exec git add {} \;
find examples -name \*.*pp -exec git add {} \;

find tests -name \*.py -exec git add {} \;

