#ifndef ALGORITHM_IPOW_ISQRT_HPP
#define ALGORITHM_IPOW_ISQRT_HPP 1

#include <cassert>
#include <cmath>  // for abs().

namespace algorithm {

// 繰返し二乗法．O(logK).
long long ipow(long long n, long long k) {
    assert(k >= 0);
    if(k == 0) return 1;
    long long &&res = ipow(n * n, k >> 1);
    if(k & 1LL) res *= n;
    return res;
}

// 平方根（ニュートン法）．
double isqrt(double x, const double eps = 1e-10) {
    assert(x >= 0.0);
    double res = 5.0;
    while(1) {
        double tmp = (x / res + res) / 2;
        if(std::abs(tmp - res) < eps) break;
        res = tmp;
    }
    return res;
}

}  // namespace algorithm

#endif
