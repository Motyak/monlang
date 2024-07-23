#ifndef VEC_UTILS_H
#define VEC_UTILS_H

#include <initializer_list>
#include <vector>
#include <set>

template <typename T>
std::vector<T> vec_union(const std::initializer_list<std::vector<T>>& vecs) {
    std::set<T> tmp;
    for (auto vec: vecs) {
        tmp.insert(vec.begin(), vec.end());
    }
    return std::vector<T>(tmp.begin(), tmp.end());
}

#endif // VEC_UTILS_H
