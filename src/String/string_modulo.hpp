#ifndef ALGORITHM_STRING_MODULO_HPP
#define ALGORITHM_STRING_MODULO_HPP 1

#include <cassert>
#include <string>

namespace algorithm {

// 文字列剰余．O(|S|).
long long string_modulo(const std::string &s, const int mod) {
    const int n = s.size();
    long long res = 0;
    for(int i = 0; i < n; ++i) {
        assert('0' <= s[i] and s[i] <= '9');
        res = (10 * res + (s[i] - '0')) % mod;
    }
    return res;
}

}  // namespace algorithm

#endif
