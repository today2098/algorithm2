#ifndef ALGORITHM_TABLE_HPP
#define ALGORITHM_TABLE_HPP 1

#include <utility>  // for move().
#include <vector>

namespace algorithm {

template <typename Type>
std::vector<Type> table(size_t n, Type val) { return std::vector<Type>(n, val); }

template <class... Args>
auto table(size_t n, Args &&...args) {
    auto val = table(std::forward<Args>(args)...);
    return std::vector<decltype(val)>(n, std::move(val));
}

}  // namespace algorithm

#endif
