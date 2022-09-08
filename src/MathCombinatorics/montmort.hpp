#ifndef ALGORITHM_MONTMORT_HPP
#define ALGORTIHM_MONTMORT_HPP 1

#include <cassert>
#include <cmath>  // for exp().
#include <vector>

namespace algorithm {

// 完全順列（mod付き）．
template <int mod>
class Montmort {
    int m_sz;
    std::vector<long long> m_a;  // m_a[k]:=(k番目のモンモール数).

public:
    Montmort() : Montmort(51e4) {}
    explicit Montmort(size_t sz) : m_sz(sz), m_a(sz) {
        static_assert(mod >= 1);
        assert(m_sz >= 2);
        m_a[0] = 1, m_a[1] = 0;
        for(int i = 2; i < m_sz; ++i) m_a[i] = (i - 1) * ((m_a[i - 2] + m_a[i - 1]) % mod) % mod;  // 隣接三項間の漸化式．
        // for(int i = 2; i < m_sz; ++i) m_a[i] = (i * m_a[i - 1] % mod + (i & 1 ? -1 : 1) + mod) % mod;  // 隣接二項間の漸化式．
    }

    static constexpr int modulus() { return mod; }
    // k個の要素を並び替えたときに完全順列となる通り数．
    long long montmort(int k) const {
        assert(1 <= k and k < m_sz);
        return m_a[k];
    }
    // 無限個の要素を並び替えたときに完全順列となる確率．
    static constexpr double convergence_probability() { return 1.0 / std::exp(1.0); }
};

}  // namespace algorithm

#endif
