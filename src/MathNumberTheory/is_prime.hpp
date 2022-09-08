#ifndef ALGORITHM_IS_PRIME_HPP
#define ALGORITHM_IS_PRIME_HPP 1

namespace algorithm {

// 素数判定．O(√N).
bool is_prime(long long n) {
    if(n < 2) return false;
    for(long long p = 2; p * p <= n; ++p) {
        if(n % p == 0) return false;
    }
    return true;
}

}  // namespace algorithm

#endif
