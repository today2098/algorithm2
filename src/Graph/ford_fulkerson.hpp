#ifndef ALGORITHM_FORD_FULKERSON_HPP
#define ALGORITHM_FORD_FULKERSON_HPP 1

#include <algorithm>  // for fill() and min().
#include <cassert>
#include <vector>

namespace algorithm {

template <typename T>
class FordFulkerson {
    struct Edge {
        int to;       // to:=(行き先ノード).
        T cap, icap;  // cap:=(容量), icap:=(元の容量).
        int rev;      // rev:=(逆辺イテレータ).
        explicit Edge(int to_, T cap_, int rev_) : to(to_), cap(cap_), icap(cap_), rev(rev_) {}
    };

    int vn;                             // vn:=(ノード数).
    std::vector<std::vector<Edge> > g;  // g[v][]:=(ノードvがもつ辺のリスト).
    bool *seen;                         // seen[v]:=(DFSでノードvを調べたか).
    T inf;

    void init() {
        for(std::vector<Edge> &es : g)
            for(Edge &e : es) e.cap = e.icap;
    }
    // 増加パスを探す．
    T dfs(int v, int t, T f) {
        if(v == t) return f;
        seen[v] = true;
        for(Edge &e : g[v]) {
            if(!seen[e.to] and e.cap > 0) {
                T res = dfs(e.to, t, std::min(f, e.cap));
                if(res > 0) {
                    e.cap -= res;
                    g[e.to][e.rev].cap += res;
                    return res;
                }
            }
        }
        return 0;
    }

public:
    // constructor.
    FordFulkerson() : FordFulkerson(0) {}
    explicit FordFulkerson(size_t vn_, T inf_ = 1e9) : vn(vn_), g(vn_), inf(inf_) {
        seen = new bool[vn]{};
    }

    // ノード数を返す．
    int size() const { return vn; }
    T infinity() const { return inf; }
    // 容量capの有向辺を追加する．
    void add_directed_edge(int from, int to, T cap) {
        assert(0 <= from and from < vn);
        assert(0 <= to and to < vn);
        assert(from != to);
        g[from].emplace_back(to, cap, g[to].size());
        g[to].emplace_back(from, 0, g[from].size() - 1);
    }
    // 容量capの無向辺を追加する．
    void add_undirected_edge(int u, int v, T cap) {
        assert(0 <= u and u < vn);
        assert(0 <= v and v < vn);
        assert(u != v);
        g[u].emplace_back(v, cap, g[v].size());
        g[v].emplace_back(u, cap, g[u].size() - 1);
    }
    // ノードsからtへの最大流を調べる．O(F*|E|).
    T max_flow(int s, int t) {
        assert(0 <= s and s < vn);
        assert(0 <= t and t < vn);
        init();
        T flow = 0;
        while(flow < inf) {
            std::fill(seen, seen + vn, false);
            T tmp = dfs(s, t, inf);
            if(tmp == 0) return flow;
            flow += tmp;
        }
        return inf;
    }
};

}  // namespace algorithm

#endif
