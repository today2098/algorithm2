#ifndef ALGORITHM_LOW_LINK_HPP
#define ALGORITHM_LOW_LINK_HPP 1

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

namespace algorithm {

class LowLink {
    int vn;                                 // vn:=(頂点数).
    std::vector<std::vector<int> > g;       // g[v][]:=(頂点vの隣接リスト).
    std::vector<int> ord;                   // ord[v]:=(DFS木における頂点vの行きかけ順序).
    std::vector<int> low;                   // low[v]:=(DFS木にて，葉方向に0回以上，後退辺を高々1回用いて到達できる頂点wにおけるord[w]の最小値).
    std::vector<int> aps;                   // aps[]:=(関節点のリスト). Articulations points.
    std::vector<std::pair<int, int> > brs;  // brs[]:=(橋のリスト). Bridges.

    void init() {
        std::fill(ord.begin(), ord.end(), -1);
        aps.clear();
        brs.clear();
    }
    void dfs(int u, int par, int &cnt) {
        ord[u] = low[u] = cnt++;
        int degree = 0;      // DFS木での頂点uにおける葉方向への出次数．
        bool is_ap = false;  // 頂点uが関節点か否か．
        for(auto v : g[u]) {
            if(ord[v] == -1) {  // 頂点vが未訪問のとき．
                degree++;
                dfs(v, u, cnt);
                low[u] = std::min(low[u], low[v]);
                if(ord[u] < low[v]) {  // 辺u-vが橋のとき．
                    if(u < v)
                        brs.emplace_back(u, v);
                    else
                        brs.emplace_back(v, u);
                }
                if(par != -1 and ord[u] <= low[v]) is_ap = true;  // 根以外で関節点のとき．
            } else if(v != par) {                                 // 辺u-vが後退辺のとき．
                low[u] = std::min(low[u], ord[v]);
            }
        }
        if(par == -1 and degree > 1) is_ap = true;  // 根が関節点のとき．
        if(is_ap) aps.push_back(u);
    }

public:
    // constructor.
    LowLink() : LowLink(0) {}
    explicit LowLink(size_t vn_) : vn(vn_), g(vn_), ord(vn_), low(vn_) {}

    // 頂点数を返す．
    int size() const { return vn; }
    // 無向辺を張る．
    void add_edge(int u, int v) {
        assert(0 <= u and u < vn);
        assert(0 <= v and v < vn);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    // 無向グラフの橋と関節点を求める．O(|V|+|E|).
    void calc() {
        init();
        int cnt = 0;
        for(int v = 0; v < vn; ++v) {
            if(ord[v] == -1) dfs(v, -1, cnt);
        }
        std::sort(aps.begin(), aps.end());
        std::sort(brs.begin(), brs.end());
    }
    // 関節点のリストを参照する．
    const std::vector<int> &get_aps() const { return aps; }
    // 橋のリストを参照する．
    const std::vector<std::pair<int, int> > &get_brs() const { return brs; }
};

}  // namespace algorithm

#endif

/**
 * 参考文献：
 * - o-treetree，グラフの関節点と橋を求めて再帰DFSを知る，HatenaBlog，<https://o-treetree.hatenablog.com/entry/2020/06/08/231258>（2022.9.13参照）.
 */
