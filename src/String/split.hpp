#ifndef ALGORITHM_SPLIT_HPP
#define ALGORITHM_SPLIT_HPP 1

#include <sstream>  // for stringstream.
#include <string>   // for getline().
#include <vector>

namespace algorithm {

// 文字列分割．指定の文字delimで分割する．
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> res;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        if(!item.empty()) res.push_back(item);
    }
    return res;
}

}  // namespace algorithm

#endif
