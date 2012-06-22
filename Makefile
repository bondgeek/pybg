
clean:
	find pybg -name \*.so -exec rm {} \;
	find pybg -name \*.pyc -exec rm {} \;
    
	rm -rf build
	rm -rf dist
	
build_ext:   
	python setup.py build_ext --inplace
    
test:
	nosetests -v pybg/tests
	
