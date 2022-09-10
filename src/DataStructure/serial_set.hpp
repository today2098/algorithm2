#ifndef ALGORITHM_SERIAL_SET_HPP
#define ALGORITHM_SERIAL_SET_HPP 1

#include <cassert>
#include <iostream>
#include <iterator>  // for prev().
#include <set>
#include <utility>  // for pair.

namespace algorithm {

// 連続した整数列の集合を管理するデータ構造．
template <typename T>
class SerialSet {
    std::set<std::pair<T, T> > m_st;  // m_st:=(整数の集合). 連続する整数列[l,r)をpair(l,r)で表現する．
    T m_inf;

public:
    // constructor.
    explicit SerialSet(T inf = 1e9) : m_inf(inf) {
        m_st.emplace(-m_inf, -m_inf + 1);  // 番兵用．
        m_st.emplace(m_inf, m_inf + 1);    // 〃
    }

    T infinity() const { return m_inf; }
    // 整数xを挿入する．
    bool insert(T x) { return insert(x, x + 1); }
    // 整数列[l,r)を挿入する．O(logN).
    bool insert(T l, T r) {
        assert(-m_inf < l and l < r and r <= m_inf);
        auto itr1 = std::prev(m_st.lower_bound(std::pair<T, T>(l + 1, l + 2)));
        auto [l1, r1] = *itr1;
        if(r <= r1) return false;  // 集合に完全に含まれている場合．
        auto itr3 = m_st.lower_bound(std::pair<T, T>(r, r + 1));
        auto itr2 = std::prev(itr3);
        auto [l2, r2] = *itr2;
        auto [l3, r3] = *itr3;
        if(l <= r1)
            l = l1;
        else
            itr1++;
        if(l3 <= r) {
            m_st.erase(itr1, ++itr3);
            m_st.emplace(l, r3);
        } else {
            m_st.erase(itr1, itr3);
            if(r <= r2)
                m_st.emplace(l, r2);
            else
                m_st.emplace(l, r);
        }
        return true;
    }
    // 整数xを削除する．
    bool erase(T x) { return erase(x, x + 1); }
    // 範囲[l,r)の整数列を削除する．O(logN).
    bool erase(T l, T r) {
        assert(-m_inf < l and l < r and r <= m_inf);
        auto itr1 = std::prev(m_st.lower_bound(std::pair<T, T>(l + 1, l + 2)));
        auto itr3 = m_st.lower_bound(std::pair<T, T>(r, r + 1));
        auto itr2 = std::prev(itr3);
        auto [l1, r1] = *itr1;
        auto [l2, r2] = *itr2;
        if(l < r1) {
            m_st.erase(itr1, itr3);
            if(l1 < l) m_st.emplace(l1, l);
        } else {
            if(itr1 == itr2) return false;  // 集合に全く含まれていない場合．
            m_st.erase(++itr1, itr3);
        }
        if(r < r2) m_st.emplace(r, r2);
        return true;
    }
    // 整数xが集合に含まれるか判定する．
    bool contains(T x) const { return contains(x, x + 1); }
    // 整数列[l,r)が集合に完全に含まれるか判定する．(logN).
    bool contains(T l, T r) const {
        assert(-m_inf < l and l < r and r <= m_inf);
        const auto &[_, pr] = *std::prev(m_st.lower_bound(std::pair<T, T>(l + 1, l + 2)));
        return r <= pr;
    }
    // 集合に含まれないx以上の整数の中で最小の値 (MEX:Minimum EXcluded value) を求める．O(logN).
    T mex(T x) const {
        assert(-m_inf < x and x < m_inf);
        const auto &[_, r] = *std::prev(m_st.lower_bound(std::pair<T, T>(x + 1, x + 2)));
        return (x < r ? r : x);
    }

    friend std::ostream &operator<<(std::ostream &os, const SerialSet &ob) {
        for(const auto &[l, r] : ob.m_st) os << "[" << l << ", " << r << ") ";
        os << std::endl;
    }
};

}  // namespace algorithm

#endif
