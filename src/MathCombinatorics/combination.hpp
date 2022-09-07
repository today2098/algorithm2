#ifndef ALGORITHM_COMBINATION_HPP
#define ALGORITHM_COMBINATION_HPP 1

#include <cassert>
#include <vector>

namespace algorithm {

// 二項係数（mod付き）．
template <int mod>
class Combination {
    int sz;                       // sz:=(要素数). 制約は sz<=mod．
    std::vector<long long> fact;  // fact[n]:=(nの階乗).
    std::vector<long long> inv;   // inv[n]:=(nの逆元).
    std::vector<long long> finv;  // finv[n]:=(nの階乗の逆元).

    void build() {
        fact[0] = fact[1] = 1;
        inv[1] = 1;
        finv[0] = finv[1] = 1;
        for(int n = 2; n < sz; ++n) {
            fact[n] = fact[n - 1] * n % mod;
            inv[n] = mod - inv[mod % n] * (mod / n) % mod;
            finv[n] = finv[n - 1] * inv[n] % mod;
        }
    }

public:
    // constructor. O(N).
    Combination() : Combination(51e4) {}
    explicit Combination(size_t sz_) : sz(sz_), fact(sz_), inv(sz_), finv(sz_) {
        static_assert(mod >= 1);
        assert(2 <= sz and sz <= mod);
        build();
    }

    static constexpr int modulus() { return mod; }
    // 階乗．
    long long factorial(int n) const {
        assert(0 <= n and n < sz);
        return fact[n];
    }
    // 逆元．
    long long inverse(int n) const {
        assert(1 <= n and n < sz);
        return inv[n];
    }
    // 階乗の逆元．
    long long inverse_fact(int n) const {
        assert(0 <= n and n < sz);
        return finv[n];
    }
    // 順列．
    long long nPk(int n, int k) const {
        assert(0 <= k and k <= n and n < sz);
        return fact[n] * finv[n - k] % mod;
    }
    // 組み合わせ．
    long long nCk(int n, int k) const {
        assert(0 <= k and k <= n and n < sz);
        return fact[n] * finv[n - k] % mod * finv[k] % mod;
    }
    // 重複組み合わせ．
    long long nHk(int n, int k) const { return nCk(k + n - 1, n - 1); }
};

using Combination998244353 = Combination<998'244'353>;
using Combination1000000007 = Combination<1'000'000'007>;

}  // namespace algorithm

#endif
