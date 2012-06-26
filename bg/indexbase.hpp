/*
 *  indexbase.hpp
 *  bondgeek
 *
 *  Created by Bart Mosley on 5/21/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */
#ifndef INDEXBASE_H
#define INDEXBASE_H

#include <ql/quantlib.hpp>

#include <bg/repository.hpp>

#include <map>
#include <string>

using namespace std;
using namespace QuantLib;

namespace bondgeek {
	
    template <class T>
	class IndexBase {
	protected:
		RelinkableHandle<YieldTermStructure>		_indexTermStructure;
		boost::shared_ptr<IborIndex>				_index;
        
	public:
		IndexBase() {
			_index = boost::shared_ptr<IborIndex>( new T(Period(3, Months), _indexTermStructure) );
		}
		IndexBase(Integer n, TimeUnit units) {
			_index = boost::shared_ptr<IborIndex>( new T(Period(n, units), _indexTermStructure) );
		}
		IndexBase(Frequency freq) {
			_index = boost::shared_ptr<IborIndex>( new T(Period(freq), _indexTermStructure) );
		}
        
		void linkTo(const boost::shared_ptr<YieldTermStructure> &yieldTermStructurePtr) {
			_indexTermStructure.linkTo(yieldTermStructurePtr);
		}
        
		const boost::shared_ptr<IborIndex> &operator()(void) { return _index; }
		const boost::shared_ptr<IborIndex> &operator()(const boost::shared_ptr<YieldTermStructure> &yieldTermStructurePtr) 
		{ 
			this->linkTo(yieldTermStructurePtr);
			return _index; 
		}
        
	};
    
	typedef IndexBase<USDLibor> USDLiborBase;
	typedef IndexBase<Euribor> EuriborBase;
    
    
	class IborIndexBase {
	protected:
		RelinkableHandle<YieldTermStructure>		_indexTermStructure;
		boost::shared_ptr<IborIndex>				_index;
		
	public:
		IborIndexBase();
		IborIndexBase(Integer n, TimeUnit units);        
		IborIndexBase(Frequency freq);
		
		void linkTo(const boost::shared_ptr<YieldTermStructure> &yieldTermStructurePtr);		
		const boost::shared_ptr<IborIndex> &operator()(void);
		
	};
	
    class USDLiborIndex : public IborIndexBase {
    protected:
    public:
        USDLiborIndex();
        USDLiborIndex(Integer n, TimeUnit units);
        USDLiborIndex(Frequency freq);
    };
}

#endif
