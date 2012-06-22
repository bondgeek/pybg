
clean:
	find bgpy -name \*.so -exec rm {} \;
	find bgpy -name \*.pyc -exec rm {} \;
    
	rm -rf build
	rm -rf dist
	
build_ext:   
	python setup.py build_ext --inplace
    
test:
	nosetests -v bgpy/tests
	
