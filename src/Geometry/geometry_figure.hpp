#ifndef ALGORITHM_GEOMETRY_FIGURE_HPP
#define ALGORITHM_GEOMETRY_FIGURE_HPP 1

#include <array>
#include <cassert>
#include <cmath>  // for sqrt().
#include <iostream>
#include <utility>  // for pair.

#include "./geometry_vector.hpp"

namespace algorithm {

namespace geometry {

struct Line {
    std::array<Point, 2> ps;

    Line() : Line(Point(), Point()) {}
    explicit Line(const Point &P, const Point &Q) : ps({P, Q}) {}

    Point &operator[](int i) {
        assert(0 <= i and i < 2);
        return ps[i];
    }
    const Point &operator[](int i) const {
        assert(0 <= i and i < 2);
        return ps[i];
    }

    friend std::ostream &operator<<(std::ostream &os, const Line &s) { return os << "{" << s[0] << ", " << s[1] << "}"; }

    // 線分の長さ．
    Type length() const { return Vector(ps[1] - ps[0]).norm(); }
    // 直線の傾き．
    Type slope() const { return (ps[1].y - ps[0].y) / (ps[1].x - ps[0].x); }
    // y切片．
    Type y_intercept() const { return ps[0].y - slope() * ps[0].x; }
};

using Segment = Line;

struct Circle {
    Point O;
    Type r;

    Circle() : O(Point()), r(1.0) {}
    explicit Circle(Type r_) : O(Point()), r(r_) {}
    explicit Circle(const Point &C_, Type r_) : O(C_), r(r_) {}

    friend std::ostream &operator<<(std::ostream &os, const Circle &C) { return os << "{O" << C.O << ", r = " << C.r << "}"; }

    // 円周の長さ．
    Type length() const { return 2 * r * PI; }
    // 円の面積．
    Type area() const { return r * r * PI; }
};

// 2点からの距離の比率m:nのアポロニウスの円．
Circle apporonius(const Point &A, const Point &B, Type m, Type n) {
    assert(!equal_to(m, n));
    Point P = (n * A + m * B) / (m + n);
    Point Q = (-n * A + m * B) / (m - n);
    Point O = (P + Q) / 2;
    Type r = Vector(P - O).norm();
    return Circle(O, r);
}

}  // namespace geometry

// using namespace geometry;

}  // namespace algorithm

#endif
