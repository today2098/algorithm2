#ifndef ALGORITHM_DOT_CROSS_HPP
#define ALGORITHM_DOT_CROSS_HPP 1

#include <utility>

namespace algorithm {

template <typename Type>
inline Type dot(Type x1, Type y1, Type x2, Type y2) { return x1 * x2 + y1 * y2; }

template <typename Type>
inline Type dot(const std::pair<Type, Type> &p, const std::pair<Type, Type> &q) { return p.first * q.first + p.second * q.second; }

template <typename Type>
inline Type dot(Type x, Type y) { return x * x + y * y; }

template <typename Type>
inline Type dot(const std::pair<Type, Type> &p) { return p.first * p.first + p.second * p.second; }

template <typename Type>
inline Type cross(Type x1, Type y1, Type x2, Type y2) { return x1 * y2 - y1 * x2; }

template <typename Type>
inline Type cross(const std::pair<Type, Type> &p, const std::pair<Type, Type> &q) { return p.first * q.second - p.second * q.first; }

}  // namespace algorithm

#endif
