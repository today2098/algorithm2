#ifndef ALGORITHM_LAZY_SEGMENT_TREE_HPP
#define ALGORITHM_LAZY_SEGMENT_TREE_HPP 1

#include <algorithm>  // for copy() and fill().
#include <cassert>
#include <functional>  // for function.
#include <vector>

namespace algorithm {

// 遅延評価セグメント木．
template <class Monoid, class Action>
class LazySegTree {
    using FM = std::function<Monoid(Monoid, Monoid)>;
    using FA = std::function<void(Monoid &, Action)>;
    using FC = std::function<void(Action &, Action)>;

    FM op;                     // Monoid op(Monoid,Monoid):=(二項演算関数). M•M.
    FA action;                 // void action(Monoid&,Action):=(作用素によるモノイド元への作用). M×A.
    FC composition;            // void composition(Action&,Action):=(作用素の合成). A•A.
    Monoid e;                  // e:=(単位元).
    Action id;                 // id:=(作用素の単位元).
    int sz;                    // sz:=(要素数).
    int n;                     // n:=(二分木の葉数).
    int depth;                 // depth:=(二分木の深さ).
    std::vector<Monoid> tree;  // tree(2n)[]:=(完全二分木). 1-based index.
    std::vector<Action> lazy;  // lazy(n)[k]:=(tree[k]の子 (tree[2k], tree[2k+1]) に対する遅延評価).

    void build() {
        n = 1, depth = 0;
        while(!(n >= sz)) n <<= 1, depth++;
        tree.assign(2 * n, e);
        lazy.assign(n, id);
    }
    void apply0(int k, const Action &d) {
        assert(1 <= k and k < 2 * n);
        action(tree[k], d);
        if(k < n) composition(lazy[k], d);
    }
    void push(int k) {
        assert(1 <= k and k < n);
        apply0(2 * k, lazy[k]);
        apply0(2 * k + 1, lazy[k]);
        lazy[k] = id;
    }
    void update(int k) {
        assert(1 <= k and k < n);
        tree[k] = op(tree[2 * k], tree[2 * k + 1]);
    }

public:
    // constructor. O(N).
    LazySegTree() {}
    explicit LazySegTree(const FM &op_, const FA &action_, const FC &composition_,
                         const Monoid &e_, const Action &id_, size_t sz_)
        : op(op_), action(action_), composition(composition_), e(e_), id(id_), sz(sz_) {
        build();
    }
    explicit LazySegTree(const FM &op_, const FA &action_, const FC &composition_,
                         const Monoid &e_, const Action &id_, const std::vector<Monoid> &v)
        : op(op_), action(action_), composition(composition_), e(e_), id(id_), sz(v.size()) {
        build();
        std::copy(v.begin(), v.end(), tree.begin() + n);
        for(int i = n - 1; i >= 1; --i) update(i);
    }

    // 要素数を返す．
    int size() const { return sz; }
    // モノイドの単位元を返す．
    Monoid identity() const { return e; }
    // 作用素の単位元を返す．
    Action action_identity() const { return id; }
    // k番目の要素をaに置き換える．O(logN).
    void set(int k, const Monoid &a) {
        assert(0 <= k and k < sz);
        k += n;
        for(int i = depth; i >= 1; --i) push(k >> i);
        tree[k] = a;
        for(int i = 1; i <= depth; ++i) update(k >> i);
    }
    // 区間[l,r)の要素をv[]に置き換える．O(N).
    void set(int l, int r, const std::vector<Monoid> &v) {
        assert(0 <= l and l <= r and r <= sz);
        assert(v.size() == r - l);
        if(l == r) return;
        l += n, r += n;
        for(int i = depth; i >= 1; --i) {
            if(((l >> i) << i) != l) push(l >> i);
            if(((r >> i) << i) != r) push((r - 1) >> i);
        }
        std::copy(v.begin(), v.end(), tree.begin() + l);
        for(int i = 1; i <= depth; ++i) {
            int l2 = (l >> i), r2 = ((r - 1) >> i);
            for(int j = l2; j <= r2; ++j) update(j);
        }
    }
    // k番目の要素を作用素dを用いて更新する．O(logN).
    void apply(int k, const Action &d) {
        assert(0 <= k and k < sz);
        k += n;
        for(int i = depth; i >= 1; --i) push(k >> i);
        action(tree[k], d);
        for(int i = 1; i <= depth; ++i) update(k >> i);
    }
    // 区間[l,r)を作用素dを用いて更新する．O(logN).
    void apply(int l, int r, const Action &d) {
        assert(0 <= l and l <= r and r <= sz);
        if(l == r) return;
        l += n, r += n;
        for(int i = depth; i >= 1; --i) {
            if(((l >> i) << i) != l) push(l >> i);
            if(((r >> i) << i) != r) push((r - 1) >> i);
        }
        for(int l2 = l, r2 = r; l2 < r2; l2 >>= 1, r2 >>= 1) {
            if(l2 & 1) apply0(l2++, d);
            if(r2 & 1) apply0(--r2, d);
        }
        for(int i = 1; i <= depth; ++i) {
            if(((l >> i) << i) != l) update(l >> i);
            if(((r >> i) << i) != r) update((r - 1) >> i);
        }
    }
    // 一点取得．O(logN).
    Monoid prod(int k) {
        assert(0 <= k and k < sz);
        k += n;
        for(int i = depth; i >= 1; --i) push(k >> i);
        return tree[k];
    }
    // 区間[l,r)の総積 (v[l]•v[l+1]•....•v[r-1]) を求める．O(logN).
    Monoid prod(int l, int r) {
        assert(0 <= l and l <= r and r <= sz);
        if(l == r) return e;
        l += n, r += n;
        for(int i = depth; i >= 1; --i) {
            if(((l >> i) << i) != l) push(l >> i);
            if(((r >> i) << i) != r) push((r - 1) >> i);
        }
        Monoid lv = e, rv = e;
        for(; l < r; l >>= 1, r >>= 1) {
            if(l & 1) lv = op(lv, tree[l++]);
            if(r & 1) rv = op(tree[--r], rv);
        }
        return op(lv, rv);
    }
    // 区間全体の総積を返す．O(1).
    Monoid prod_all() const { return tree[1]; }
    // jud(prod(l,-))=trueとなる区間の最右位値を二分探索する．
    // ただし要素列には単調性があり，またjud(e)=trueであること．O(logN).
    int most_right(const std::function<bool(Monoid)> &jud, int l) const {
        assert(jud(e));
        assert(0 <= l and l <= sz);
        if(l == sz) return sz;
        l += n;
        for(int i = depth; i >= 1; --i) push(l >> i);
        Monoid val = e;
        do {
            while(!(l & 1)) l >>= 1;
            Monoid &&tmp = op(val, tree[l]);
            if(!jud(tmp)) {
                while(l < n) {
                    push(l);
                    l *= 2;
                    Monoid &&tmp2 = op(val, tree[l]);
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
    int most_left(const std::function<bool(Monoid)> &jud, int r) const {
        assert(jud(e));
        assert(0 <= r and r <= sz);
        if(r == 0) return 0;
        r += n;
        for(int i = depth; i >= 1; --i) push((r - 1) >> i);
        Monoid val = e;
        do {
            r--;
            while(r > 1 and r & 1) r >>= 1;
            Monoid &&tmp = op(tree[r], val);
            if(!jud(tmp)) {
                while(r < n) {
                    push(r);
                    r = 2 * r + 1;
                    Monoid &&tmp2 = op(tree[r], val);
                    if(jud(tmp2)) val = tmp2, r--;
                }
                return r - n + 1;
            }
            val = tmp;
        } while((r & -r) != r);  // (x&-x)==xのとき，xは2の階乗数．
        return 0;
    }
    void reset() {
        std::fill(tree.begin(), tree.end(), e);
        std::fill(lazy.begin(), lazy.end(), id);
    }
};

}  // namespace algorithm

#endif
