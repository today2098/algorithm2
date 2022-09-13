#ifndef ALGORITHM_STRONGLY_CONNECTED_COMPONENTS_HPP
#define ALGORITHM_STRONGLY_CONNECTED_COMPONENTS_HPP 1

#include <algorithm>
#include <cassert>
#include <stack>
#include <vector>

namespace algorithm {

// 強連結成分分解 (SCC:Strongly Connected Components)．
class SCC {
    int m_vn;                            // m_vn:=(ノード数).
    std::vector<std::vector<int> > m_g;  // m_g[v][]:=(頂点vの隣接リスト).

public:
    SCC() : SCC(0) {}
    explicit SCC(int vn) : m_vn(vn), m_g(vn) {}

    // 頂点数を返す．
    int size() const { return m_vn; }
    // 有向辺を張る．
    void add_edge(int from, int to) {
        assert(0 <= from and from < m_vn);
        assert(0 <= to and to < m_vn);
        m_g[from].push_back(to);
    }
    // return pair of (# of SCCs, SCC id).
    std::pair<int, std::vector<int> > get_scc_id() const {
        std::vector<int> ord(m_vn, -1), low(m_vn), ids(m_vn);
        int now_ord = 0, num_groups = 0;
        std::stack<int> visited;
        auto dfs = [&](auto self, int v) -> void {
            ord[v] = low[v] = now_ord++;
            visited.push(v);
            for(auto to : m_g[v]) {
                if(ord[to] == -1) {
                    self(self, to);
                    low[v] = std::min(low[v], low[to]);
                } else {
                    low[v] = std::min(low[v], ord[to]);
                }
            }
            if(low[v] == ord[v]) {
                while(true) {
                    auto u = visited.top();
                    visited.pop();
                    ord[u] = m_vn;
                    ids[u] = num_groups;
                    if(u == v) break;
                }
                num_groups++;
            }
        };
        for(int v = 0; v < m_vn; ++v) {
            if(ord[v] == -1) dfs(dfs, v);
        }
        for(auto &x : ids) x = num_groups - 1 - x;
        return {num_groups, ids};
    }
    // 有向グラフを強連結成分分解する．
    std::vector<std::vector<int> > decompose() const {
        const auto &&[num_groups, ids] = get_scc_id();
        std::vector<int> counts(num_groups, 0);
        for(auto x : ids) counts[x]++;
        std::vector<std::vector<int> > sccs(num_groups);
        for(int i = 0; i < num_groups; ++i) sccs[i].reserve(counts[i]);
        for(int v = 0; v < m_vn; ++v) sccs[ids[v]].push_back(v);
        return sccs;
    }
};

}  // namespace algorithm

#endif
