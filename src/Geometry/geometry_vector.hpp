#ifndef ALGORITHM_GEOMETRY_POINT_HPP
#define ALGORITHM_GEOMETRY_POINT_HPP 1

#include <cmath>  // for abs(), acos(), atan2(), cos(), sin() and sqrt().
#include <iostream>
#include <utility>  // for pair.
#include <vector>

namespace algorithm {

namespace geometry {

using Type = double;

const Type INF = 1e18;
const Type EPS = 1e-10;
const Type PI = std::acos(-1.0);

inline bool equal_to(const Type &a, const Type &b) { return std::abs(a - b) < EPS; }
inline bool less_than(const Type &a, const Type &b) { return a - b < -EPS; }
inline bool more_than(const Type &a, const Type &b) { return a - b > EPS; }
inline bool no_more(const Type &a, const Type &b) { return a - b < EPS; }
inline bool no_less(const Type &a, const Type &b) { return a - b > -EPS; }

inline Type to_radian(Type deg) { return deg / 180.0 * PI; }
inline Type to_degree(Type rad) { return rad / PI * 180.0; }

struct Vector {
    Type x, y;

    Vector() : Vector(0.0, 0.0) {}
    Vector(Type x_, Type y_) : x(x_), y(y_) {}

    Vector operator+() const { return Vector(*this); }
    Vector operator-() const { return Vector(-x, -y); }
    Vector operator*(Type n) const { return Vector(x * n, y * n); }
    Vector operator/(Type n) const { return Vector(x / n, y / n); }
    Vector &operator*=(Type n) {
        x *= n, y *= n;
        return *this;
    }
    Vector &operator/=(Type n) {
        x /= n, y /= n;
        return *this;
    }
    Vector &operator+=(const Vector &a) {
        x += a.x, y += a.y;
        return *this;
    }
    Vector &operator-=(const Vector &a) {
        x -= a.x, y -= a.y;
        return *this;
    }

    friend Vector operator*(Type n, const Vector &a) { return a * n; }
    friend Vector operator+(const Vector &a, const Vector &b) { return Vector(a.x + b.x, a.y + b.y); }
    friend Vector operator-(const Vector &a, const Vector &b) { return Vector(a.x - b.x, a.y - b.y); }
    friend bool operator<(const Vector &a, const Vector &b) { return (equal_to(a.x, b.x) ? less_than(a.y, b.y) : less_than(a.x, b.x)); }
    friend bool operator>(const Vector &a, const Vector &b) { return (equal_to(a.x, b.x) ? more_than(a.y, b.y) : more_than(a.x, b.x)); }
    friend bool operator<=(const Vector &a, const Vector &b) { return (equal_to(a.x, b.x) ? no_more(a.y, b.y) : no_more(a.x, b.x)); }
    friend bool operator>=(const Vector &a, const Vector &b) { return (equal_to(a.x, b.x) ? no_less(a.y, b.y) : no_less(a.x, b.x)); }
    friend bool operator==(const Vector &a, const Vector &b) { return (equal_to(a.x, b.x) and equal_to(a.y, b.y)); }
    friend bool operator!=(const Vector &a, const Vector &b) { return !(a == b); }
    friend std::ostream &operator<<(std::ostream &os, const Vector &a) { return os << "(" << a.x << ", " << a.y << ")"; }

    // ノルム．
    Type norm() const { return std::sqrt(dot(*this, *this)); }
    // 単位ベクトル．
    Vector unit() const { return *this / norm(); }
    // x軸となす角を返す．[-π, π].
    Type arg() const { return std::atan2(y, x); }
    // x軸となす角を返す．[0, 2π].
    Type arg2() const {
        Type res = arg();
        if(less_than(res, 0.0)) res += PI;
        return res;
    }

    // 内積．
    friend inline Type dot(const Vector &a, const Vector &b) { return a.x * b.x + a.y * b.y; }
    // 外積．
    friend inline Type cross(const Vector &a, const Vector &b) { return a.x * b.y - a.y * b.x; }
};

using Point = Vector;
using Polygon = std::vector<Point>;

// 半時計回りに角度arg（ラジアン単位）回転させる．
void rotate(Vector &a, Type arg) {
    Type nx = a.x * std::cos(arg) - a.y * std::sin(arg);
    Type ny = a.x * std::sin(arg) + a.y * std::cos(arg);
    a.x = nx, a.y = ny;
}
inline void rotate90(Vector &a) { a = Vector(-a.y, a.x); }

struct Complex : public Point {
    Complex() : Point() {}
    Complex(const Point &P) : Point(P) {}
    Complex(Type x_, Type y_) : Point(x_, y_) {}

    Complex &operator*=(const Complex &z) {
        const auto [tx, ty] = *this;
        x = tx * z.x - ty * z.y;
        y = tx * z.y + ty * z.x;
        return *this;
    }
    Complex &operator/=(const Complex &z) {
        const auto [tx, ty] = *this;
        x = tx * z.x - ty * z.y;
        y = tx * z.y + ty * z.x;
        return *this;
    }

    friend Complex operator*(const Complex &z, const Complex &w) { return Complex(z.x * w.x - z.y * w.y, z.x * w.y + z.y * w.x); }
    friend Complex operator/(const Complex &z, const Complex &w) { return Complex(z / (w.x * w.x + w.y * w.y)) * w; }

    // 共役複素数．Conjugate complex number.
    Complex conj() const { return Complex(x, -y); }
};

// 極形式．大きさr，偏角argの複素数を返す．
inline Complex polar(Type r, Type arg) { return Complex(r * std::cos(arg), r * std::sin(arg)); }
// 極形式(r,θ)に変換．θ∈[-π, π].
inline std::pair<Type, Type> convert_polor(const Complex &z) { return {z.norm(), z.arg()}; }
// 極形式(r,θ)に変換．θ∈[0, 2π].
inline std::pair<Type, Type> convert_polor2(const Complex &z) { return {z.norm(), z.arg2()}; }

}  // namespace geometry

// using namespace geometry;

}  // namespace algorithm

#endif
