#ifndef ALGORITHM_MOD_INV_HPP
#define ALGORITHM_MOD_INV_HPP 1

#include <cassert>
#include <numeric>  // for gcd().
#include <utility>  // for swap().

namespace algorithm {

// 逆元．a^-1 (mod m)を求める（ただし，aとmは互いに素）．
long long mod_inv(long long a, int m) {
    // assert(m >= 1 and std::gcd(a, m) == 1);
    assert(m >= 1);
    long long b = m, u = 1, v = 0;
    while(b) {
        long long t = a / b;
        a -= t * b, u -= t * v;
        std::swap(a, b), std::swap(u, v);
    }
    return (u % m + m) % m;
}

}  // namespace algorithm

#endif  // ALGORITHM_MOD_INV_HPP
