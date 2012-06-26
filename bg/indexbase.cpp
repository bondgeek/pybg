/*
 *  indexbase.cpp
 *  pybg
 *
 *  Created by BART MOSLEY on 6/26/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#include <bg/indexbase.hpp>

namespace bondgeek {
    
    IborIndexBase::IborIndexBase() {
        _index = boost::shared_ptr<IborIndex>( new USDLibor(Period(3, Months), _indexTermStructure) );
    }
    
    IborIndexBase::IborIndexBase(Integer n, TimeUnit units) {
        _index = boost::shared_ptr<IborIndex>( new USDLibor(Period(n, units), _indexTermStructure) );
    }
    
    IborIndexBase::IborIndexBase(Frequency freq) {
        _index = boost::shared_ptr<IborIndex>( new USDLibor(Period(freq), _indexTermStructure) );
    }
    
    void IborIndexBase::linkTo(const boost::shared_ptr<YieldTermStructure> &yieldTermStructurePtr) {
        _indexTermStructure.linkTo(yieldTermStructurePtr);
    }
    
    const boost::shared_ptr<IborIndex> &IborIndexBase::operator()(void) 
    { 
        return _index; 
    }
    
    
    USDLiborIndex::USDLiborIndex() {
        _index = boost::shared_ptr<IborIndex>( new USDLibor(Period(3, Months), _indexTermStructure) );
    }
    
    USDLiborIndex::USDLiborIndex(Integer n, TimeUnit units) {
        _index = boost::shared_ptr<IborIndex>( new USDLibor(Period(n, units), _indexTermStructure) );
    }
    
    USDLiborIndex::USDLiborIndex(Frequency freq) {
        _index = boost::shared_ptr<IborIndex>( new USDLibor(Period(freq), _indexTermStructure) );
    }
    

}
