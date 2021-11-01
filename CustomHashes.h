#ifndef CUSTOM_HASHES_H
#define CUSTOM_HASHES_H

    #include <vector>
    #include <functional>

    #include <boost/range/iterator_range_core.hpp>
    #include <boost/functional/hash.hpp>
    /*
    #include"AStarHeader.h"


    size_t HashVector::operator ()(const std::vector<long>& v) const
    {
        return boost::hash_range(v.begin(), v.end());
    }*/

    template<typename Type>
    struct HashVector
    {
        size_t operator ()(const Type& v) const
        {
            return boost::hash_range(v.begin(), v.end());
        }
    };

#endif