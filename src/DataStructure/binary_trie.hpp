#ifndef ALGORITHM_BINARY_TRIE_HPP
#define ALGORITHM_BINARY_TRIE_HPP 1

#include <cassert>
#include <vector>

namespace algorithm {

// 非負整数値を扱うTrie木．
template <typename T = unsigned, int B = 32>  // T:データ型, B:ビット長.
class BinaryTrie {
    struct Node {
        int cnt;      // cnt:=(自身を根とする部分木に含まれる要素数).
        Node *ch[2];  // ch[]:=(子のポインタ). 二分木．
        Node() : cnt(0), ch({nullptr, nullptr}) {}
    };

    Node *root;  // root:=(根のポインタ).

    void delete_tree(Node *rt) {
        if(rt == nullptr) return;
        for(int i = 0; i < 2; ++i) {
            delete_tree(rt->ch[i]);
            rt->ch[i] = nullptr;
        }
        delete rt;
        rt = nullptr;
    }
    Node *add(Node *p, T x, int shift = B - 1) {
        if(p == nullptr) p = new Node;
        p->cnt++;
        if(shift < 0) return p;
        bool flag = (x >> shift) & (T)1;
        p->ch[flag] = add(p->ch[flag], x, shift - 1);
        return p;
    }
    Node *sub(Node *p, T x, int shift = B - 1) {
        assert(p != nullptr);
        p->cnt--;
        if(p->cnt == 0) {
            delete_tree(p);
            return p = nullptr;
        }
        if(shift < 0) return p;
        bool flag = (x >> shift) & (T)1;
        p->ch[flag] = sub(p->ch[flag], x, shift - 1);
        return p;
    }
    T get_min(Node *p, T bias = 0, int shift = B - 1) const {
        assert(p != nullptr);
        if(shift < 0) return 0;
        bool flag = (bias >> shift) & (T)1;
        if(p->ch[flag] == nullptr) flag = !flag;
        return get_min(p->ch[flag], bias, shift - 1) | ((T)flag << shift);
    }
    T get(Node *p, int k, T bias = 0, int shift = B - 1) const {
        assert(p != nullptr);
        if(shift < 0) return 0;
        bool flag = (bias >> shift) & (T)1;
        int m = (p->ch[flag] == nullptr ? 0 : p->ch[flag]->cnt);
        if(k < m) {
            return get(p->ch[flag], k, bias, shift - 1) | ((T)flag << shift);
        } else {
            flag = !flag;
            return get(p->ch[flag], k - m, bias, shift - 1) | ((T)flag << shift);
        }
    }
    int count_lower(Node *p, T x, int shift = B - 1) const {
        if(p == nullptr or shift < 0) return 0;
        bool flag = (x >> shift) & (T)1;
        return ((flag and p->ch[0] != nullptr) ? p->ch[0]->cnt : 0) + count_lower(p->ch[flag], x, shift - 1);
    }
    void dfs(std::ostream &os, Node *p, int i, bool bits[]) const {
        if(i == B) {
            os << "[";
            for(int j = 0; j < B; ++j) os << (bits[j] ? 1 : 0);
            os << "] (num:" << p->cnt << ")" << std::endl;
            return;
        }
        for(int j = 0; j < 2; ++j) {
            if(p->ch[j] != nullptr) {
                bits[i] = j;
                dfs(os, p->ch[j], i + 1, bits);
            }
        }
    }

public:
    // constructor.
    BinaryTrie() : root(nullptr) {
        static_assert(B >= 1);
    }
    ~BinaryTrie() {
        clear();
    }

    // 集合内でk番目に小さい値を取得．O(B).
    T operator[](int k) const {
        assert(0 <= k and k < size());
        return get(root, k);
    }

    // 要素数を返す．O(1).
    int size() const { return (root == nullptr ? 0 : root->cnt); }
    // 要素が空かどうか．O(1).
    bool empty() const { return root == nullptr; }
    // 値xの要素数を返す．O(B).
    int count(T x) const {
        if(root == nullptr) return 0;
        Node *p = root;
        for(int i = B - 1; i >= 0; --i) {
            p = p->ch[(x >> i) & (T)1];
            if(p == nullptr) return 0;
        }
        return p->cnt;
    }
    // 値xを集合に1つ追加．O(B).
    void insert(T x) { root = add(root, x); }
    // 値xを集合から1つ削除．O(B).
    void erace(T x) {
        assert(count(x) >= 1);
        root = sub(root, x);
    }
    // 集合内で値xとXORしたときに最小となる値を取得．O(B).
    T min_element(T x = 0) const { return get_min(root, x); }
    // 集合内で値xとXORしたときに最大となる値を取得．O(B).
    T max_element(T x = 0) const { return get_min(root, ~x); }
    // 集合内で値xとXORしたときにk番目に小さい値を取得．O(B).
    T kth_element(int k, T x = 0) const {
        assert(0 <= k and k < size());
        return get(root, k, x);
    }
    // 集合内で値x以上の最小の要素の番号を取得．O(B).
    int lower_bound(T x) const { return count_lower(root, x); }
    // 集合内で値xより大きい最小の要素の番号を取得．O(B).
    int upper_bound(T x) const { return count_lower(root, x + 1); }
    void clear() {
        delete_tree(root);
        root = nullptr;
    }

    friend std::ostream &operator<<(std::ostream &os, const BinaryTrie &ob) {
        if(ob.empty()) return os;
        bool bits[B] = {};
        ob.dfs(os, ob.root, 0, bits);
    }
};

}  // namespace algorithm

#endif

/**
 * 参考文献
 * - kazuma，非負整数値を扱うTrieについて，Hatena Blog，<https://kazuma8128.hatenablog.com/entry/2018/05/06/022654>（2022.9.11参照）．
 */
