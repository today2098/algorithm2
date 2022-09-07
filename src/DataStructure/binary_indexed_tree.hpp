#ifndef ALGORITHM_BINARY_INDEXED_TREE_HPP
#define ALGORITHM_BINARY_INDEXED_TREE_HPP 1

#include <algorithm>  // for copy() and fill().
#include <cassert>
#include <vector>

namespace algorithm {

// Binary Indexed Tree.
template <typename T>
class BIT {
    int sz;               // sz:=(要素数).
    std::vector<T> tree;  // 1-based index.

    void build() {
        for(int i = 1; i < sz; ++i) {
            int j = i + (i & -i);
            if(j <= sz) tree[j] += tree[i];
        }
    }

public:
    // constructor. O(N).
    BIT() : BIT(0){};
    explicit BIT(size_t sz_) : sz(sz_), tree(sz_ + 1, 0) {}
    explicit BIT(const std::vector<T> &v) : sz(v.size()), tree(v.size() + 1) {
        std::copy(v.begin(), v.end(), tree.begin() + 1);
        build();
    }

    int size() const { return sz; }
    // 全要素をaで埋める．O(N).
    void fill(T a = 0) {
        std::fill(tree.begin() + 1, tree.end(), a);
        if(a == 0) return;
        build();
    }
    // k番目の要素にaを足す．O(logN).
    void add(int k, T a) {
        assert(1 <= k and k <= sz);
        for(; k <= sz; k += (k & -k)) tree[k] += a;
    }
    // 区間[1,r]の総和を求める．O(logN).
    T sum(int r) const {
        assert(0 <= r and r <= sz);
        T res = 0;
        for(; r > 0; r -= (r & -r)) res += tree[r];
        return res;
    }
    // 区間[l,r]の総和を求める．O(logN).
    T sum(int l, int r) const {
        assert(1 <= l and l <= r and r <= sz);
        return sum(r) - sum(l - 1);
    }
};

}  // namespace algorithm

#endif
