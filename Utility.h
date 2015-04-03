#pragma once
#include <ctime>
#include <stdlib.h>
#include <iostream>

#include <unistd.h>
#include <functional>
#include <unordered_map>
#include <set>
#include <unordered_set>




typedef unsigned long long uint64;
typedef signed int			int32;
typedef unsigned int			uint32;


//static int seed = 0;
//static int GetRand(int min, int max){
//    srand(time(nullptr) + seed);
//    seed = rand();
//    int dis = seed % (max - min);
//    return dis + min;
//}

struct Position{
    int x;
    int y;
};




template <typename K, typename V>
class CnHashMap : public std::unordered_map<K, V>
{
public:
    typedef typename CnHashMap<K, V>::const_iterator const_iterator;
    typedef typename CnHashMap<K, V>::iterator iterator;

public:

    V get(const K& key, V defaultValue) const
    {
        const_iterator itor = this->find(key);
        if (itor == this->end())
            return defaultValue;
        return itor->second;
    }

    bool has(const K& key) const
    {
        const_iterator itor = this->find(key);
        return (itor != this->end());
    }

    typedef std::function< bool(const V&) > value_predicator;

    const_iterator find_value(value_predicator predicator) const
    {
        for (auto itor = this->begin(); itor != this->end(); ++itor)
        {
            if (predicator(itor->second))
            {
                return itor;
            }
        }
        return this->end();
    }

    iterator find_value(value_predicator predicator)
    {
        for (auto itor = this->begin(); itor != this->end(); ++itor)
        {
            if (predicator(itor->second))
            {
                return itor;
            }
        }
        return this->end();
    }
};
