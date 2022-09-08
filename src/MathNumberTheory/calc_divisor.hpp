#ifndef ALGORITHM_CALC_DIVISOR_HPP
#define ALGORITHM_CALC_DIVISOR_HPP 1

#include <algorithm>  // for sort().
#include <cassert>
#include <vector>

namespace algorithm {

// 約数列挙．O(√N).
template <typename Type>
std::vector<Type> calc_divisor(Type n) {
    assert(n >= 0);
    std::vector<Type> res;  // res[]:=(自然数nの約数の集合).
    for(Type p = 1; p * p <= n; ++p) {
        if(n % p == 0) {
            res.push_back(p);
            Type q = n / p;
            if(q != p) res.push_back(q);
        }
    }
    std::sort(res.begin(), res.end());
    return res;
}

}  // namespace algorithm

#endif
