#include <vector>
#include <functional>

#include <boost/range/iterator_range_core.hpp>
#include <boost/functional/hash.hpp>

#include"AStarHeader.h"


size_t HashVector::operator ()(const std::vector<long>& v) const
{
    return boost::hash_range(v.begin(), v.end());
}
