#ifndef ALGORITHM_PRINT_HPP
#define ALGORITHM_PRINT_HPP 1

#include <iostream>
#include <string>

namespace algorithm {

template <class Class>
void line(const Class &a, std::string delim = " ") {
    int cnt = 0;
    for(const auto &elem : a) std::cout << (cnt++ == 0 ? "" : delim) << elem;
    std::cout << std::endl;
}

// #define DEBUG
#ifdef DEBUG

#define debug(...) debug_internal(__LINE__, #__VA_ARGS__, __VA_ARGS__)
#define dline(a) dline_internal(__LINE__, #a, a);

template <typename T, class... Args>
void debug_internal(int l, const char *context, T &&first, Args &&...args) {
    constexpr const char *open_brackets = (sizeof...(args) == 0 ? "" : "(");
    constexpr const char *close_brackets = (sizeof...(args) == 0 ? "" : ")");
    std::cerr << "[L" << l << "] " << open_brackets << context << close_brackets << ": "
              << open_brackets << std::forward<T>(first);
    ((std::cerr << ", " << std::forward<Args>(args)), ...);
    std::cerr << close_brackets << std::endl;
}

template <class Class>
void dline_internal(int l, const char *context, const Class &a) {
    std::cerr << "[L" << l << "] " << context << ": ";
    int cnt = 0;
    for(const auto &elem : a) std::cerr << (cnt++ == 0 ? "" : " ") << elem;
    std::cerr << std::endl;
}

#else

#define debug(...) void(0)
#define dline(a) void(0)

#endif

}  // namespace algorithm

#endif
