#ifndef ALGORITHM_FLOOR_SUM_HPP
#define ALGORITHM_FLOOR_SUM_HPP 1

#include <cassert>

namespace algorithm {

// sum_{x=0}^{n-1} floor((a*x+b)/m)を求める．O(loga+logm).
long long floor_sum(long long n, long long m, long long a, long long b) {
    assert(0 <= n and n < (1LL << 32));
    assert(1 <= m and m < (1LL << 32));
    long long res = 0;
    if(a < 0) {
        long long a2 = (a % m + m) % m;
        res -= n * (n - 1) / 2 * ((a2 - a) / m);
        a = a2;
    }
    if(b < 0) {
        long long b2 = (b % m + m) % m;
        res -= n * ((b2 - b) / m);
        b = b2;
    }
    if(a >= m) {
        res += n * (n - 1) / 2 * (a / m);
        a %= m;
    }
    if(b >= m) {
        res += n * (b / m);
        b %= m;
    }
    long long y_max = a * n + b;
    if(y_max >= m) res += floor_sum(y_max / m, a, m, y_max % m);
    return res;
}

}  // namespace algorithm

#endif

/**
 * 参考文献
 * - HNJ，Floor Sum (ACL Practice Contest C)，Qiita，<https://qiita.com/HNJ/items/564f15316719209df73c>（2022.9.17参照）．
 * - rsk0315，sum of floor of liner の解説をするよ，Hatena Blog，<https://rsk0315.hatenablog.com/entry/2020/12/13/231307>（2022.9.17参照）．
 * - yosupo，C - Floor Sum 解説，AtCoder，<https://atcoder.jp/contests/practice2/editorial/579>（2022.9.17参照）．
 */
