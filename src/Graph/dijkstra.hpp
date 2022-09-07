#ifndef ALGORITHM_DIJKSTRA_HPP
#define ALGORITHM_DIJKSTRA_HPP 1

#include <algorithm>  // for fill() and reverse().
#include <cassert>
#include <functional>  // for greater.
#include <queue>       // for priority_queue.
#include <utility>     // for pair.
#include <vector>

namespace algorithm {

template <typename T>
class Dijkstra {
    template <typename Type>
    using r_priority_queue = std::priority_queue<Type, std::vector<Type>, std::greater<Type> >;
    struct Edge {
        int to;
        T cost;
    };

    int m_vn;                             // m_vn:=(ノード数).
    std::vector<std::vector<Edge> > m_g;  // m_g[v][]:=(ノードvが始点である有向辺のリスト).
    std::vector<T> m_d;                   // m_d[t]:=(ノードsからtへの最短距離).
    std::vector<int> m_pre;               // m_pre[t]:=(ノードtを訪問する直前のノード番号). 逆方向経路．
    T m_inf;

public:
    // constructor.
    Dijkstra() : Dijkstra(0) {}
    explicit Dijkstra(size_t vn, T inf = 1e9) : m_vn(vn), m_g(vn), m_d(vn, inf), m_pre(vn, -1), m_inf(inf) {}

    // ノード数を返す．
    int size() const { return m_vn; }
    T infinity() const { return m_inf; }
    // 重み付き有向辺を張る．
    void add_edge(int from, int to, T cost) {
        assert(0 <= from and from < m_vn);
        assert(0 <= to and to < m_vn);
        m_g[from].push_back((Edge){to, cost});
    }
    // ノードsから各ノードへの最短距離を求める．O(|E|*log|V|).
    void dijkstra(int s) {
        assert(0 <= s and s < m_vn);
        std::fill(m_d.begin(), m_d.end(), m_inf);
        m_d[s] = 0;
        std::fill(m_pre.begin(), m_pre.end(), -1);
        r_priority_queue<std::pair<T, int> > pque;
        pque.emplace(0, s);
        while(!pque.empty()) {
            auto [dist, v] = pque.top();
            pque.pop();
            if(m_d[v] < dist) continue;
            for(const Edge &e : m_g[v]) {
                if(m_d[e.to] > m_d[v] + e.cost) {
                    m_d[e.to] = m_d[v] + e.cost;
                    m_pre[e.to] = v;
                    pque.emplace(m_d[e.to], e.to);
                }
            }
        }
    }
    // ノードsからtへの最短距離を返す．
    T distance(int t) const {
        assert(0 <= t and t < m_vn);
        return m_d[t];
    }
    // ノードsからtへの最短経路を復元する．
    std::vector<int> get_path(int t) const {
        assert(0 <= t and t < m_vn);
        std::vector<int> path;
        for(; t != -1; t = m_pre[t]) path.push_back(t);
        std::reverse(path.begin(), path.end());
        return path;
    }
};

}  // namespace algorithm

#endif
