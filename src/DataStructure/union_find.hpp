#ifndef ALGORITHM_UNION_FIND_HPP
#define ALGORITHM_UNION_FIND_HPP 1

#include <algorithm>  // for fill().
#include <cassert>
#include <utility>  // for swap().
#include <vector>

namespace algorithm {

// 素集合データ構造．
class UnionFind {
    int m_vn;                // m_vn:=(ノード数).
    int m_gn;                // m_gn:=(グループ数).
    std::vector<int> m_par;  // m_par[v]:=(ノードvの親番号). 0未満の場合，vは親で，値の絶対値はグループサイズを表す．

public:
    // constructor.
    UnionFind() : UnionFind(0) {}
    explicit UnionFind(size_t vn) : m_vn(vn), m_gn(vn), m_par(vn, -1) {}

    // ノード数を返す．
    int get_vn() const { return m_vn; };
    // グループ数を返す．
    int get_gn() const { return m_gn; };
    // ノードvの親番号を返す．
    int root(int v) {
        assert(0 <= v and v < m_vn);
        if(m_par[v] < 0) return v;
        return m_par[v] = root(m_par[v]);
    }
    // ノードvが属するグループのサイズを返す．
    int size(int v) {
        assert(0 <= v and v < m_vn);
        return -m_par[root(v)];
    }
    // ノードuとvが同じグループか判定する．
    bool same(int u, int v) {
        assert(0 <= u and u < m_vn);
        assert(0 <= v and v < m_vn);
        return root(u) == root(v);
    }
    // ノードu, vが属するそれぞれのグループを結合する．
    bool unite(int u, int v) {
        assert(0 <= u and u < m_vn);
        assert(0 <= v and v < m_vn);
        u = root(u), v = root(v);
        if(u == v) return false;                // Do nothing.
        if(size(u) < size(v)) std::swap(u, v);  // Merge technique.
        m_par[u] += m_par[v];
        m_par[v] = u;
        m_gn--;
        return true;
    }
    void reset() {
        m_gn = m_vn;
        std::fill(m_par.begin(), m_par.end(), -1);
    }
};

}  // namespace algorithm

#endif
