#ifndef ALGORITHM_BINARY_INDEXED_TREE_2D_HPP
#define ALGORITHM_BINARY_INDEXED_TREE_2D_HPP 1

#include <algorithm>  // for copy() and fill().
#include <cassert>
#include <vector>

namespace algorithm {

// Binary Indexed Tree 2D.
template <typename T>
class BIT2D {
    int h;                             // h:=(height).
    int w;                             // w:=(width).
    std::vector<std::vector<T> > dat;  // 1-based index.

    void build() {
        for(int i = 1; i <= h; ++i)
            for(int j = 1; j <= w; ++j) {
                int ni = i + (i & -i), nj = j + (j & -j);
                bool flag1 = (ni <= h), flag2 = (nj <= w);
                if(flag1) dat[ni][j] += dat[i][j];
                if(flag2) dat[i][nj] += dat[i][j];
                if(flag1 and flag2) dat[ni][nj] -= dat[i][j];
            }
    }

public:
    // constructor. O(H*W).
    BIT2D() : BIT2D(0, 0) {}
    explicit BIT2D(size_t h_, size_t w_) : h(h_), w(w_), dat(h_ + 1, std::vector<T>(w_ + 1, 0)) {}
    explicit BIT2D(const std::vector<std::vector<T> > &dat_)
        : h(dat_.size()), w(dat_[0].size()), dat(dat_.size() + 1, std::vector<T>(dat_[0].size() + 1)) {
        for(int i = 0; i < h; ++i) std::copy(dat_[i].begin(), dat_[i].end(), dat[i + 1].begin() + 1);
        build();
    }

    int height() const { return h; }
    int width() const { return w; }
    // 全要素をaで埋める．O(H*W).
    void fill(T a = 0) {
        for(int i = 1; i <= h; ++i) std::fill(dat[i].begin() + 1, dat[i].end(), a);
        if(a == 0) return;
        build();
    }
    // 座標(x,y)にaを加算する．O((logH)*logW).
    void add(int y, int x, T a) {
        assert(1 <= y and y <= h);
        assert(1 <= x and x <= w);
        for(int i = y; i <= h; i += (i & -i))
            for(int j = x; j <= w; j += (j & -j)) dat[i][j] += a;
    }
    // 区間[1,y]かつ[1,x]の総和を求める．O((logH)*logW).
    T sum(int y, int x) const {
        assert(0 <= y and y <= h);
        assert(0 <= x and x <= w);
        T res = 0;
        for(int i = y; i > 0; i -= (i & -i))
            for(int j = x; j > 0; j -= (j & -j)) res += dat[i][j];
        return res;
    }
    // 区間[y,yy]かつ[x,xx]の総和を求める．
    T sum(int y, int x, int yy, int xx) const {
        assert(1 <= y and y <= yy and yy <= h);
        assert(1 <= x and x <= xx and xx <= w);
        return sum(yy, xx) - sum(yy, x - 1) - sum(y - 1, xx) + sum(y - 1, x - 1);
    }
};

}  // namespace algorithm

#endif
