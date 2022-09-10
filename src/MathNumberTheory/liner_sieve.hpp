#ifndef ALGORITHM_LINEAR_SIEVE_HPP
#define ALGORITHM_LINEAR_SIEVE_HPP 1

#include <algorithm>  // for sort().
#include <cassert>
#include <map>
#include <vector>

namespace algorithm {

// 線形篩．
class LinearSieve {
    int m_mx;                   // m_mx:=(篩にかける最大の自然数).
    std::vector<int> m_lpf;     // m_lpf[n]:=(自然数nの最小の素因数). Least prime factor. m_lpf[n]==nのとき，nは素数．
    std::vector<int> m_primes;  // m_primes[]:=(素数のリスト).

public:
    // constructor. mx以下の自然数を篩にかける．O(N).
    LinearSieve() : LinearSieve(51e4) {}
    explicit LinearSieve(int mx) : m_mx(mx), m_lpf(mx + 1, -1) {
        assert(m_mx >= 0);
        for(int p = 2; p <= m_mx; ++p) {
            if(m_lpf[p] == -1) {
                m_lpf[p] = p;
                m_primes.push_back(p);
            }
            int sz = m_primes.size();
            for(int i = 0; i < sz and m_primes[i] <= m_lpf[p] and p * m_primes[i] <= m_mx; ++i) m_lpf[p * m_primes[i]] = m_primes[i];
        }
    }

    // 素数判定．O(1).
    bool is_prime(int n) const {
        assert(0 <= n and n <= m_mx);
        return m_lpf[n] == n;
    }
    // 自然数nの最小の素因数を返す．O(1).
    int get_lpf(int n) const {
        assert(2 <= n and n <= m_mx);
        return m_lpf[n];
    }
    // 高速素因数分解．O(logN).
    std::map<int, int> prime_factorize(int n) const {
        assert(1 <= n and n <= m_mx);
        std::map<int, int> res;
        while(n > 1) {
            res[m_lpf[n]]++;
            n /= m_lpf[n];
        }
        return res;
    }
    // 高速約数列挙．
    std::vector<int> divisors(int n) const {
        assert(1 <= n and n <= m_mx);
        std::vector<int> res({1});
        const auto &&pf = prime_factorize(n);
        for(const auto &[p, cnt] : pf) {
            const int sz = res.size();
            int b = 1;
            for(int i = 0; i < cnt; ++i) {
                b *= p;
                for(int j = 0; j < sz; ++j) res.push_back(res[i] * b);
            }
        }
        std::sort(res.begin(), res.end());
        return res;
    }
    const std::vector<int> &get_primes() const { return m_primes; }
};

}  // namespace algorithm

#endif
