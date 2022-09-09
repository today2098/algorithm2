#ifndef ALGORITHM_SEGMENT_TREE_HPP
#define ALGORITHM_SEGMENT_TREE_HPP 1

#include <algorithm>  // for copy() and fill().
#include <cassert>
#include <functional>  // for function.
#include <vector>

namespace algorithm {

template <typename T>
class SegmentTree {
    using F = std::function<T(T, T)>;

    F op;                 // T op(T,T):=(二項演算関数).
    T e;                  // e:=(単位元).
    int sz;               // sz:=(要素数).
    int n;                // n:=(葉の数).
    std::vector<T> tree;  // tree[]:=(完全二分木). 1-based index.

    void build() {
        n = 1;
        while(n < sz) n <<= 1;
        tree.assign(2 * n, e);
    }

public:
    // constructor. O(N).
    SegmentTree(){};
    explicit SegmentTree(const F &op_, const T &e_, size_t sz_) : op(op_), e(e_), sz(sz_) {
        build();
    }
    explicit SegmentTree(const F &op_, const T &e_, std::vector<T> &v) : op(op_), e(e_), sz(v.size()) {
        build();
        std::copy(v.begin(), v.end(), tree.begin() + n);
        for(int i = n - 1; i >= 1; --i) tree[i] = op(tree[i << 1], tree[(i << 1) | 1]);
    }

    // 要素数を返す．
    int size() const { return sz; }
    // 単位元を返す．
    T identity() const { return e; }
    // k番目の要素をaに置き換える．O(logN).
    void set(int k, const T &a) {
        assert(0 <= k and k < sz);
        k += n;
        tree[k] = a;
        while(k >>= 1) tree[k] = op(tree[k << 1], tree[(k << 1) | 1]);
    }
    // k番目の要素をaを加える．O(logN).
    void set(int k, const T &a) {
        assert(0 <= k and k < sz);
        k += n;
        tree[k] += a;
        while(k >>= 1) tree[k] = op(tree[k << 1], tree[(k << 1) | 1]);
    }
    // 一点取得．O(1).
    T prod(int k) const {
        assert(0 <= k and k < sz);
        return tree[k + n];
    }
    // 区間[l,r)の総積（v[l]•v[l+1]•....•v[r-1]）を求める．O(logN).
    T prod(int l, int r) const {
        assert(0 <= l and l <= r and r <= sz);
        T lv = e, rv = e;
        l += n, r += n;
        while(l < r) {
            if(l & 1) lv = op(lv, tree[l++]);
            if(r & 1) rv = op(tree[--r], rv);
            l >>= 1, r >>= 1;
        }
        return op(lv, rv);
    }
    // 区間全体の総積を返す．O(1).
    T prod_all() const { return tree[1]; }
    // jud(prod(l,-))=trueとなる区間の最右位値を二分探索する．
    // ただし要素列には単調性があり，またjud(e)=trueであること．O(logN).
    int most_right(const std::function<bool(T)> &jud, int l) const {
        assert(jud(e) == true);
        assert(0 <= l and l <= sz);
        if(l == sz) return sz;
        T val = e;
        l += n;
        do {
            while(!(l & 1)) l >>= 1;
            T &&tmp = op(val, tree[l]);
            if(!jud(tmp)) {
                while(l < n) {
                    l <<= 1;
                    T &&tmp2 = op(val, tree[l]);
                    if(jud(tmp2)) val = tmp2, l++;
                }
                return l - n;
            }
            val = tmp, l++;
        } while((l & -l) != l);  // (x&-x)==xのとき，xは2の階乗数．
        return sz;
    }
    // jud(prod(-,r))=trueとなる区間の最左位値を二分探索する．
    // ただし要素列には単調性があり，またjud(e)=trueであること．O(logN).
    int most_left(const std::function<bool(T)> &jud, int r) const {
        assert(jud(e) == true);
        assert(0 <= r and r <= sz);
        if(r == 0) return 0;
        T val = e;
        r += n;
        do {
            r--;
            while(r > 1 and r & 1) r >>= 1;
            T &&tmp = op(tree[r], val);
            if(!jud(tmp)) {
                while(r < n) {
                    r = (r << 1) | 1;
                    T &&tmp2 = op(tree[r], val);
                    if(jud(tmp2)) val = tmp2, r--;
                }
                return r - n + 1;
            }
            val = tmp;
        } while((r & -r) != r);  // (x&-x)==xのとき，xは2の階乗数．
        return 0;
    }
    void reset() { std::fill(tree.begin(), tree.end(), e); }
};

}  // namespace algorithm

#endif
