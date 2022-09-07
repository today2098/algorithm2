#ifndef ALGORITHM_BINARY_INDEXED_TREE_ZERO_HPP
#define ALGORITHM_BINARY_INDEXED_TREE_ZERO_HPP 1

#include <algorithm>  // for fill().
#include <cassert>
#include <vector>

namespace algorithm {

// Binary Indexed Tree (0-based index).
template <typename T>
class BIT0 {
    int sz;               // sz:=(要素数).
    std::vector<T> tree;  // 0-based index.

    void build() {
        for(int i = 0; i < sz - 1; ++i) {
            int j = i | (i + 1);
            if(j < sz) tree[j] += tree[i];
        }
    }

public:
    // constructor. O(N).
    BIT0() : BIT0(0){};
    explicit BIT0(size_t sz_) : sz(sz_), tree(sz_, 0) {}
    explicit BIT0(const std::vector<T> &v) : sz(v.size()), tree(v) {
        build();
    }

    int size() const { return sz; }
    // 全要素をaで埋める．O(N).
    void fill(T a = 0) {
        std::fill(tree.begin(), tree.end(), a);
        if(a == 0) return;
        build();
    }
    // k番目の要素にaを足す．O(logN).
    void add(int k, T a) {
        assert(0 <= k and k < sz);
        for(; k < sz; k |= k + 1) tree[k] += a;
    }
    // 区間[0,r)の和を求める．O(logN).
    T sum(int r) const {
        assert(0 <= r and r <= sz);
        T res = 0;
        for(r = r - 1; r >= 0; r = (r & (r + 1)) - 1) res += tree[r];
        return res;
    }
    // 区間[l,r)の和を求める．O(logN).
    T sum(int l, int r) const {
        assert(0 <= l and l <= r and r <= sz);
        return sum(r) - sum(l);
    }
};

}  // namespace algorithm

#endif
