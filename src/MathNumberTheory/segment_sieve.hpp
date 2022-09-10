#ifndef ALGORITHM_SEGMENT_SIEVE_HPP
#define ALGORITHM_SEGMENT_SIEVE_HPP 1

#include <algorithm>  // for max() and sort().
#include <cassert>
#include <cmath>  // for sqrt().
#include <map>
#include <vector>

namespace algorithm {

// 区間篩．
class SegmentSieve {
    long long l, r;
    long long sr;                                  // sr:=√r.
    std::vector<long long> small;                  // small[n]:=(区間[2,√r)の自然数nの最小の素因数).
    std::vector<std::map<long long, int> > large;  // large[n-l][]:=(区間[l,r)の自然数nの区間[2,√r)における素因数).
    std::vector<long long> aux;                    // aux[n-l]:=(large[n-l][]の積).

    void build() {
        sr = std::sqrt(r) + 5;
        small.assign(sr, -1);
        for(long long p = 2; p < sr; ++p) small[p] = p;
        large.resize(r - l);
        aux.assign(r - l, 1);
        for(long long p = 2; p * p < r; ++p) {
            if(small[p] == p) {
                for(long long n = p * p; n < sr; n += p) small[n] = p;
                for(long long n = std::max<long long>(2, (l + p - 1) / p) * p; n < r; n += p) {
                    long long tmp = n;
                    while(tmp % p == 0 and aux[n - l] * aux[n - l] <= r) {
                        large[n - l][p]++;
                        aux[n - l] *= p;
                        tmp /= p;
                    }
                }
            }
        }
    }

public:
    // constructor. 区間[l,r)の自然数を篩にかける．制約の目安は大体 2<=l<r<=1e12, r-l<=1e6．
    SegmentSieve() : SegmentSieve(2, 3) {}
    explicit SegmentSieve(long long l_, long long r_) : l(l_), r(r_) {
        assert(2 <= l and l < r);
        build();
    }

    // 素数判定．O(1).
    bool is_prime(long long n) const {
        assert(l <= n and n < r);
        return large[n - l].size() == 0;
    }
    // 高速素因数分解．
    std::map<long long, int> prime_factorize(long long n) const {
        assert(l <= n and n < r);
        std::map<long long, int> res = large[n - l];
        n /= aux[n - l];
        if(n >= sr) {
            res[n]++;
            return res;
        }
        while(n > 1) {
            res[small[n]]++;
            n /= small[n];
        }
        return res;
    }
    // 高速約数列挙．
    std::vector<long long> divisors(long long n) const {
        assert(l <= n and n < r);
        std::vector<long long> res({1});
        if(n == 1) return res;
        const auto &&pf = prime_factorize(n);
        for(const auto &[p, cnt] : pf) {
            const int sz = res.size();
            long long b = 1;
            for(int i = 0; i < cnt; ++i) {
                b *= p;
                for(int j = 0; j < sz; ++j) res.push_back(res[i] * b);
            }
        }
        std::sort(res.begin(), res.end());
        return res;
    }
};

}  // namespace algorithm

#endif

/**
 * 参考文献
 * - rsk0315_h4x，エラトステネスの篩に基づく高速な素因数分解，Qiita，<https://qiita.com/rsk0315_h4x/items/ff3b542a4468679fb409>（2022.9.10参照）．
 */
