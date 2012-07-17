/*
 *  repository.hpp
 *  QuantLibFun
 *
 *  Created by BART MOSLEY on 5/16/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */
#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <map>
#include <string>

using namespace std;

namespace bondgeek {
    
    template <class K, class T>
    class Repository {
    protected:
        std::map<K, T> contents;
        
    public:
        Repository() {};
        
        int add_item(K key, T item) {
            if (contents.count(key)>0)
                return 0;
            contents[key] = item;
            return contents.size();
        }
        
        const T& get_item(K key) {
            if (contents.count(key) > 0)
                return contents[key];
            
            const T& rc = T();
            return rc;
        }
        
        const T& operator[](K key) { return get_item(key); }
    };
        
}
#endif
