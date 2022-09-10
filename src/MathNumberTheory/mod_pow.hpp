#ifndef ALGORITHM_MOD_POW_HPP
#define ALGORITHM_MOD_POW_HPP 1

#include <cassert>

namespace algorithm {

// 繰返し二乗法（mod付き）．n^k (mod m)を求める．O(logK).
long long mod_pow(long long n, long long k, int m) {
    assert(k >= 0 and m >= 1);
    n %= m;
    long long res = 1;
    while(k > 0) {
        if(k & 1LL) res = res * n % m;
        n = n * n % m;
        k >>= 1;
    }
    return res;
}

}  // namespace algorithm

#endif
