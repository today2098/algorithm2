#ifndef ALGORITHM_GEOMETRY_FUNCTION_HPP
#define ALGORITHM_GEOMETRY_FUNCTION_HPP 1

#include <algorithm>  // for sort().
#include <cmath>      // for abs().
#include <iostream>
#include <utility>  // for pair.
#include <vector>

#include "./geometry_figure.hpp"
#include "./geometry_vector.hpp"

namespace algorithm {

namespace geometry {

// 点と線分，直線，円の位置関係．

int simple_ccw(const Point &A, const Point &B, const Point &P) {
    Vector b = B - A, p = P - A;
    if(more_than(cross(b, p), 0.0)) return 1;   // A->Bの反時計方向にP．
    if(less_than(cross(b, p), 0.0)) return -1;  // A->Bの時計方向にP．
    return 0;                                   // 一直線上．
}
int ccw(const Point &A, const Point &B, const Point &P) {
    Vector b = B - A, p = P - A;
    if(more_than(cross(b, p), 0.0)) return 1;      // A->Bの反時計方向にP．counter clockwise.
    if(less_than(cross(b, p), 0.0)) return -1;     // A->Bの時計方向にP．clockwise.
    if(more_than(dot(p, p), dot(b, b))) return 2;  // A->B->Pの順に一直線上．online front.
    if(less_than(dot(b, p), 0.0)) return -2;       // P->A->Bの順に一直線上．online back.
    return 0;                                      // A->P->Bの順に一直線上．on segment.
}

bool on_line(const Line &l, const Point &P) { return (simple_ccw(l[0], l[1], P) == 0); }
bool on_segment(const Segment &s, const Point &P) { return (ccw(s[0], s[1], P) == 0); }

// 垂線の足．
Point projection(const Line &l, const Point &P) {
    Vector a = (l[1] - l[0]);
    return l[0] + a * (dot(P - l[0], a) / dot(a, a));
}
// 対象点．
Point reflection(const Line &l, const Point &P) { return P + 2.0 * (projection(l, P) - P); }

Type dist_ptop(const Point &P, const Point &Q) { return Vector(Q - P).norm(); }
Type dist_ptol(const Point &P, const Line &l) {
    Point H = projection(l, P);
    return dist_ptop(P, H);
}
Type dist_ptos(const Point &P, const Segment &s) {
    Point H = projection(s, P);
    if(on_segment(s, H)) return dist_ptop(P, H);
    return std::min(dist_ptop(P, s[0]), dist_ptop(P, s[1]));
}

// 三角形と点の包含関係．
int contain(const Point &A, const Point &B, const Point &C, const Point &P) {
    int flag1 = ccw(A, B, P), flag2 = ccw(B, C, P), flag3 = ccw(C, A, P);
    if(flag1 == 0 or flag2 == 0 or flag3 == 0) return 1;       // on.
    if(flag1 == 1 and flag2 == 1 and flag3 == 1) return 2;     // in.
    if(flag1 == -1 and flag2 == -1 and flag3 == -1) return 2;  // in.
    return 0;                                                  // out.
}
// 円と点の包含関係．
int contain(const Circle &C, const Point &P) {
    Type dif = dist_ptop(C.O, P) - C.r;
    if(less_than(dif, 0.0)) return 2;  // in.
    if(equal_to(dif, 0.0)) return 1;   // on.
    return 0;                          // out.
}

// 直線の直交判定．
bool jud_orthogonal(const Line &l, const Line &m) {
    Vector b = l[1] - l[0], d = m[1] - m[0];
    return equal_to(dot(b, d), 0.0);
}
// 直線の平行判定．
bool jud_parallel(const Line &l, const Line &m) {
    Vector b = l[1] - l[0], d = m[1] - m[0];
    return equal_to(cross(b, d), 0.0);
}
// 線分の交点判定．
bool jud_cross(const Segment &s, const Segment &t) {
    Vector b = s[1] - s[0], c1 = t[0] - s[0], d1 = t[1] - s[0];
    Vector a = s[0] - s[1], c2 = t[0] - s[1], d2 = t[1] - s[1];
    return (no_more(cross(b, c1) * cross(b, d1), 0.0) and no_more(cross(a, c2) * cross(a, d2), 0.0));
}

// 線分，直線，円の交点を求める関数．

std::vector<Point> cross_point(const Line &l, const Line &m) {
    std::vector<Point> ps;
    if(jud_parallel(l, m)) return ps;
    Vector a = l[1] - l[0], b = m[1] - m[0];
    ps.push_back(l[0] + a / cross(b, a) * cross(b, m[0] - l[0]));
    return ps;
}
std::vector<Point> cross_point(const Circle &C, const Line &l) {
    std::vector<Point> ps;
    Point H = projection(l, C.O);
    int flag = contain(C, H);
    if(flag == 0) {
    } else if(flag == 1) {
        ps.push_back(H);
    } else {
        Type dist = dist_ptop(C.O, H);
        Type arg = std::acos(dist / C.r);
        ps.push_back(C.O + polar(C.r, arg));
        ps.push_back(C.O + polar(C.r, -arg));
    }
    return ps;
}
std::vector<Point> cross_point(Circle C, Circle D) {
    std::vector<Point> ps;
    if(less_than(C.r, D.r)) std::swap(C, D);
    Type dist = dist_ptop(C.O, D.O);
    if(more_than(dist, C.r + D.r)) {  // 離れている場合．
        // No points.
    } else if(equal_to(dist, C.r + D.r)) {  // 外接する場合．
        Vector base = Vector(D.O - C.O).unit();
        ps.push_back(C.O + base * C.r);
    } else if(less_than(std::abs(C.r - D.r), dist) and less_than(dist, C.r + D.r)) {  // 交差する場合．
        Type tmp = C.r - (C.r + D.r - dist) / 2.0;
        Type arg = std::acos(tmp / C.r);
        ps.push_back(C.O + polar(C.r, arg));
        ps.push_back(C.O + polar(C.r, -arg));
    } else if(equal_to(dist, std::abs(C.r - D.r))) {  // 内接する場合．
        Vector base = Vector(D.O - C.O).unit();
        ps.push_back(C.O + base * C.r);
    } else if(less_than(dist, std::abs(C.r - D.r))) {  // 内部にある場合．
        // No points.
    }
    return ps;
}

// 多角形に関する関数，アルゴリズム．

// 点を反時計周りにソート．O(NlogN).
void vertices_sort(Polygon &pol) {
    const int n = pol.size();
    Polygon tmp = pol;
    int p = 0;
    for(int i = 1; i < n; ++i) {
        if(less_than(tmp[i].y, tmp[p].y))
            p = i;
        else if(no_more(tmp[i].y, tmp[p].y) and less_than(tmp[i].x, tmp[p].x))
            p = i;
    }
    std::vector<std::pair<Type, int> > vp(n);
    for(int i = 0; i < n; ++i) {
        if(i == p)
            vp[i] = std::pair<Type, int>(-10.0, i);
        else
            vp[i] = std::pair<Type, int>(Vector(tmp[i] - tmp[p]).arg(), i);
    }
    std::sort(vp.begin(), vp.end());
    for(int i = 0; i < n; ++i) pol[i] = tmp[vp[i].second];
}

Type area(const Point &A, const Point &B, const Point &C) { return std::abs(cross(B - A, C - A)) / 2.0; }  // 三角形の面積．
Type area(const Polygon &pol) {                                                                            // O(N).
    const int n = pol.size();
    Type res = 0.0;
    for(int i = 0; i < n; ++i) res += cross(pol[i], pol[(i + 1) % n]);
    return res / 2.0;
}

// 多角形と点の包含関係．O(N).
int contain(const Polygon &pol, const Point &P) {
    const int n = pol.size();
    bool jud = false;
    for(int i = 0; i < n; ++i) {
        Vector A = pol[i] - P, B = pol[(i + 1) % n] - P;
        if(equal_to(cross(A, B), 0.0) and no_more(dot(A, B), 0.0)) return 1;  // on.
        if(A.y > B.y) std::swap(A, B);
        if(no_more(A.y, 0.0) and less_than(0.0, B.y) and more_than(cross(A, B), 0.0)) jud = !jud;
    }
    return (jud ? 2 : 0);  // in or out.
}

// 凸性判定．O(N).
bool is_convex(const Polygon &pol) {
    const int n = pol.size();
    for(int i = 0; i < n; ++i) {
        if(ccw(pol[i], pol[(i + 1) % n], pol[(i + 2) % n]) == -1) return false;
    }
    return true;
}

// アンドリューのアルゴリズム．凸包．O(N*logN).
Polygon andrew_scan(Polygon &pol, bool eliminate_duplication = true) {
    const int cmp = (eliminate_duplication ? geometry::EPS : -geometry::EPS);
    const int n = pol.size();
    Polygon res(2 * n);
    int k = 0;
    std::sort(pol.begin(), pol.end());
    for(int i = 0; i < n; ++i) {
        while(k >= 2 and cross(res[k - 1] - res[k - 2], pol[i] - res[k - 2]) < cmp) k--;
        res[k++] = pol[i];
    }
    int t = k + 1;
    for(int i = n - 2; i >= 0; --i) {
        while(k >= t and cross(res[k - 1] - res[k - 2], pol[i] - res[k - 2]) < cmp) k--;
        res[k++] = pol[i];
    }
    res.resize(k - 1);
    return res;
}

// 凸多角形の切断．O(N).
Polygon convex_cut(const Polygon &pol, const Line &l) {
    assert(is_convex(pol));
    const int n = pol.size();
    Polygon res;
    for(int i = 0; i < n; ++i) {
        Point P = pol[i], Q = pol[(i + 1) % n];
        if(ccw(l[0], l[1], P) != -1) {
            if(res.size() == 0 or P != *res.rbegin()) res.push_back(P);
        }
        if(ccw(l[0], l[1], P) * ccw(l[0], l[1], Q) < 0) {
            std::vector<Point> temp = cross_point(Line(P, Q), l);
            if(temp.size() == 0) continue;
            if(res.size() == 0 or temp[0] != *res.rbegin()) res.push_back(temp[0]);
        }
    }
    return res;
}

}  // namespace geometry

// using namespace geometry;

}  // namespace algorithm

#endif
