#ifndef ALGORITHM_PRIM_HPP
#define ALGORITHM_PRIM_HPP 1

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

namespace algorithm {

template <typename T>
class Prim {
    struct Edge {
        int to;
        T cost;
    };

    int m_vn;                                // m_vn:=(ノード数).
    int m_n;                                 // m_n:=(葉の数).
    std::vector<std::vector<Edge> > m_g;     // m_g[v][]:=(ノードvがもつ辺のリスト).
    std::vector<std::pair<T, int> > m_heap;  // m_heap[]:=(完全二分木). 1-based index.
    T m_inf;

    void shiht_up(int i) {
        assert(m_n <= i and i < 2 * m_n);
        while(i >>= 1) m_heap[i] = std::min(m_heap[2 * i], m_heap[2 * i + 1]);
    }
    void push(T cost, int k) {
        assert(0 <= k and k < m_vn);
        if(cost >= m_heap[k + m_n].first) return;
        m_heap[k + m_n] = std::pair<T, int>(cost, k);
        shiht_up(k + m_n);
    }
    void pop() {
        auto [_, k] = m_heap[1];
        if(k == -1) return;
        m_heap[k + m_n] = std::pair<T, int>(m_inf, -1);
        shiht_up(k + m_n);
    }
    std::pair<T, int> top() const { return m_heap[1]; }

public:
    // constructor.
    Prim() : Prim(0) {}
    explicit Prim(size_t vn, T inf = 1e9) : m_vn(vn), m_g(vn), m_inf(inf) {
        m_n = 1;
        while(m_n < m_vn) m_n <<= 1;
        m_heap.assign(2 * m_n, std::pair<T, int>(m_inf, -1));
    }

    // ノード数を返す．
    int size() const { return m_vn; }
    // 重み付き無向辺を張る．
    void add_edge(int u, int v, T cost) {
        assert(0 <= u and u < m_vn);
        assert(0 <= v and v < m_vn);
        m_g[u].push_back((Edge){v, cost});
        m_g[v].push_back((Edge){u, cost});
    }
    // ノードvを含む最小全域木のコストを求める．O(|E|*log|V|).
    T prim(int v = 0) {
        assert(0 <= v and v < m_vn);
        T res = 0;
        bool seen[m_vn] = {};
        push(0, v);
        while(1) {
            auto [cost, u] = top();
            if(u == -1) break;
            pop();
            if(seen[u]) continue;
            seen[u] = true;
            res += cost;
            for(const Edge &e : m_g[u]) {
                if(!seen[e.to]) push(e.cost, e.to);
            }
        }
        return res;
    }
};

}  // namespace algorithm

#endif
