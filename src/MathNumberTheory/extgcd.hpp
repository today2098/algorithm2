#ifndef ALGORITHM_EXTGCD_HPP
#define ALGORITHM_EXTGCD_HPP 1

#include <cassert>
#include <utility>
#include <vector>

namespace algorithm {

// 拡張ユークリッド互除法．ax+by=GCD(a,b) を満たす整数の組(x,y)を格納する．O(logN).
template <typename Type>
Type extgcd(Type a, Type b, Type &x, Type &y) {
    if(b == 0) {
        x = 1, y = 0;
        return a;
    }
    Type d = extgcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
}

// 中国剰余定理 (Chinese Remainder Theorem)．
//　x≡r[i] (mod. m[i]) の解を x≡R (mod. M) として {R,M} を求める．解なしのときは {0,-1}．
std::pair<long long, long long> crt(const std::vector<long long> &vr, const std::vector<long long> &vm) {
    assert(vr.size() == vm.size());
    if(vr.empty()) return {0, 1};
    const int n = vr.size();
    long long r = vr[0], m = vm[0];
    for(int i = 1; i < n; ++i) {
        long long p, q, tr = vr[i], tm = vm[i];
        if(m < tm) std::swap(r, tr), std::swap(m, tm);  // prevent overflow.
        long long d = extgcd(m, tm, p, q);              // p is inv of m/d (mod. m/d).
        if((tr - r) % d != 0) return {0, -1};
        long long md = m / d;
        long long tmp = (tr - r) / d % md * p % md;
        r += m * tmp, m *= md;
    }
    r %= m;
    if(r < 0) r += m;
    return {r, m};
}

}  // namespace algorithm

#endif

/**
 * 参考文献
 * drken，中国剰余定理 (CRT) の解説とそれを用いる問題のまとめ，<https://qiita.com/drken/items/ae02240cd1f8edfc86fd>（2022.9.16参照）．
 */
