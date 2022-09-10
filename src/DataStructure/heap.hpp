#ifndef ALGORITHM_HEAP_HPP
#define ALGORITHM_HEAP_HPP 1

#include <algorithm>  // for copy().
#include <cassert>
#include <functional>  // for function.
#include <utility>     // for swap().
#include <vector>

namespace algorithm {

template <typename T>
class Heap {
    using F = std::function<bool(T &, T &)>;

    F comp;               // bool comp(T&,T&):=(比較演算関数).
    int sz;               // sz:=(要素数).
    int depth;            // depth:=(二分木の高さ).
    std::vector<T> tree;  // tree[]:=(二分木). 1-based index.

    int shift_up(int k) {
        assert(1 <= k and k <= sz);
        if(k == 1) return 1;
        int par = k / 2;
        if(comp(tree[par], tree[k])) return k;
        std::swap(tree[par], tree[k]);
        return par;
    }
    void all_shift_up(int k) {
        while(1) {
            int next = shift_up(k);
            if(next == k) return;
            k = next;
        }
    }
    int shift_down(int k) {
        assert(1 <= k and k <= sz);
        if(k > sz / 2) return k;
        int l = 2 * k, r = 2 * k + 1;
        if(r > sz or comp(tree[l], tree[r])) {
            if(comp(tree[k], tree[l])) return k;
            std::swap(tree[k], tree[l]);
            return l;
        } else {
            if(comp(tree[k], tree[r])) return k;
            std::swap(tree[k], tree[r]);
            return r;
        }
    }
    void all_shift_down(int k) {
        while(1) {
            int next = shift_down(k);
            if(next == k) return;
            k = next;
        }
    }
    void heap_sort() {
        int k = sz / 2;
        for(int i = k; i >= 1; --i) all_shift_down(i);
    }

public:
    // constructor. O(N*logN).
    Heap() : Heap([](const T &a, const T &b) -> bool { return true; }, std::vector<T>(0)) {}
    explicit Heap(const F &comp_) : Heap(comp_, std::vector<T>(0)) {}
    explicit Heap(const std::vector<T> &v) : Heap([](const T &a, const T &b) -> bool { return true; }, v) {}
    explicit Heap(const F &comp_, const std::vector<T> &v) : comp(comp_), sz(v.size()), depth(-1), tree(v.size() + 1) {
        if(sz > 0) {
            std::copy(v.begin(), v.end(), tree.begin() + 1);
            depth = 0;
            while((1 << (depth + 1)) - 1 < sz) depth++;
            heap_sort();
        }
    }
    ~Heap() {
        std::vector<T>().swap(tree);
    }

    int size() const { return sz; }
    bool empty() const { return sz == 0; }
    // 先頭の要素を返す．O(1).
    T top() const {
        assert(sz > 0);
        return tree[1];
    }
    // 挿入．O(logN).
    void insert(const T &a) {
        tree.push_back(a), sz++;
        if(depth == -1) depth = 0;
        while((1 << (depth + 1)) - 1 < sz) depth++;
        all_shift_up(sz);
    }
    // 先頭削除．O(logN).
    T pop() {
        assert(sz > 0);
        T res = tree[1];
        tree[1] = tree[sz];
        tree.pop_back(), sz--;
        if(sz == 0) {
            depth = -1;
            return res;
        }
        while(sz < (1 << depth)) depth--;
        all_shift_down(1);
        return res;
    }
    // ヒープソート．O(N).
    void heap_sort(const F &comp_) {
        comp = comp_;
        heap_sort();
    }
};

}  // namespace algorithm

#endif
