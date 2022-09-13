#ifndef ALGORITHM_MANACHER_HPP
#define ALGORITHM_MANACHER_HPP 1

#include <cassert>
#include <vector>

namespace algorithm {

// 最長回文 (Longest Palindromic Substring)．
template <class Class>
class Manacher {
    int n;  // n:=(配列サイズ).
    // radius[2*k]:=(k文字目を中心とする奇数長の最長回文の半径),
    // radius[2*k-1]:=(k-1文字目とk文字目の間を中心とする偶数長の最長回文の半径).
    std::vector<int> radius;

    void manacher(const Class &t) {
        int i = 0, j = 0;
        while(i < 2 * n - 1) {
            while(0 <= i - j and i + j < 2 * n - 1 and t[i - j] == t[i + j]) j++;
            radius[i] = j;
            int k = 1;
            while(i - k >= 0 and radius[i - k] + k < j) {
                radius[i + k] = radius[i - k];
                k++;
            }
            i += k, j -= k;
        }
    }

public:
    // constructor. 引数はSTLのシーケンスコンテナ．O(|S|).
    Manacher() : Manacher(Class()) {}
    explicit Manacher(const Class &s) : n(s.size()), radius(2 * s.size()) {
        Class t(2 * n, 0);
        for(int i = 0; i < n; ++i) t[2 * i] = s[i];
        manacher(t);
    }

    // k文字目を中心とする奇数長の最長回文の半径を返す．
    int get_odd(int k) const {
        assert(0 <= k and k < n);
        return (radius[2 * k] + 1) / 2;
    }
    // k-1文字目とk文字目の間を中心とする偶数長の最長回文の半径を返す．
    int get_even(int k) const {
        assert(1 <= k and k < n);
        return radius[2 * k - 1] / 2;
    }
    // 部分列s[l:r]が回文か判定する．
    bool is_palindrome(int l, int r) const {
        assert(0 <= l and l < r and r <= n);
        int mid = (l + r) / 2;
        if((r - l) & 1)
            return get_odd(mid) == (r - l + 1) / 2;
        else
            return get_even(mid) == (r - l) / 2;
    }
};

}  // namespace algorithm

#endif
