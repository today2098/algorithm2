#ifndef ALGORITHM_BINARY_INDEXED_TREE_2D_ZERO_HPP
#define ALGORITHM_BINARY_INDEXED_TREE_2D_ZERO_HPP 1

#include <algorithm>  // for fill().
#include <cassert>
#include <vector>

namespace algorithm {

// Binary Indexed Tree 2D (0-based index).
template <typename T>
class BIT2D0 {
    int h;                             // h:=(height).
    int w;                             // w:=(width).
    std::vector<std::vector<T> > dat;  // 0-based index.

    void build() {
        for(int i = 0; i < h; ++i)
            for(int j = 0; j < w; ++j) {
                int ni = i | (i + 1), nj = j | (j + 1);
                bool flag1 = (ni < h), flag2 = (nj < w);
                if(flag1) dat[ni][j] += dat[i][j];
                if(flag2) dat[i][nj] += dat[i][j];
                if(flag1 and flag2) dat[ni][nj] -= dat[i][j];
            }
    }

public:
    // constructor. O(H*W).
    BIT2D0() : BIT2D0(0, 0) {}
    explicit BIT2D0(size_t h_, size_t w_) : h(h_), w(w_), dat(h_, std::vector<T>(w_, 0)) {}
    explicit BIT2D0(const std::vector<std::vector<T> > &dat_) : h(dat_.size()), w(dat_[0].size()), dat(dat_) {
        build();
    }

    int height() const { return h; }
    int width() const { return w; }
    // 全要素をaで埋める．O(H*W).
    void fill(T a = 0) {
        for(int i = 0; i < h; ++i) std::fill(dat[i].begin(), dat[i].end(), a);
        if(a == 0) return;
        build();
    }
    // 座標(x,y)にaを加算する．O((logH)*logW).
    void add(int y, int x, T a) {
        assert(0 <= y and y < h);
        assert(0 <= x and x < w);
        for(int i = y; i < h; i |= i + 1)
            for(int j = x; j < w; j |= j + 1) dat[i][j] += a;
    }
    // 区間[0,y)かつ[0,x)の総和を求める．O((logH)*logW).
    T sum(int y, int x) const {
        assert(0 <= y and y <= h);
        assert(0 <= x and x <= w);
        T res = 0;
        for(int i = y - 1; i >= 0; i = (i & (i + 1)) - 1)
            for(int j = x - 1; j >= 0; j = (j & (j + 1)) - 1) res += dat[i][j];
        return res;
    }
    // 区間[y,yy)かつ[x,xx)の総和を求める．
    T sum(int y, int x, int yy, int xx) const {
        assert(0 <= y and y <= yy and yy <= h);
        assert(0 <= x and x <= xx and xx <= w);
        return sum(yy, xx) - sum(yy, x) - sum(y, xx) + sum(y, x);
    }
};

}  // namespace algorithm

#endif
