
clean:
	find pybg -name \*.so -exec rm {} \;
	find pybg -name \*.pyc -exec rm {} \;
    
	rm -rf build
	rm -rf dist

clean_win:
	del pybg\*.cpp

build_ext:   
	python setup.py build_ext --inplace

build_win:
	python setup_win.py build_ext --inplace
	
test:
	nosetests -v pybg/tests
	
test0:
	nosetests -v pybg/tests/test_curves.py
	
install:
	python setup.py build_ext
	python setup.py install
	