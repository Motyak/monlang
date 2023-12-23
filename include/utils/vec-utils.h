
#ifndef VEC_UTILS_H
#define VEC_UTILS_H

#include <initializer_list>
#include <vector>
#include <set>

template <typename T>
std::vector<T> vec_concat(const std::vector<T>& vecA, const std::vector<T>& vecB) {
    std::vector<T> res;
    res.insert(res.end(), vecA.begin(), vecA.end());
    res.insert(res.end(), vecB.begin(), vecB.end());
    return res;
}

template <typename T>
std::vector<T> vec_union(const std::initializer_list<std::vector<T>>& vecs) {
    std::vector<T> res;
    for (auto vec: vecs) {
        res.insert(res.end(), vec.begin(), vec.end());
    }
    std::set<T> tmp(res.begin(), res.end()); // remove potential duplicates
    res.assign(tmp.begin(), tmp.end());
    return res;
}

template <typename T>
std::vector<std::vector<T>> vec_union(const std::initializer_list<std::vector<std::vector<T>>>& vecs) {
    std::vector<std::vector<T>> res;
    for (auto vec: vecs) {
        res.insert(res.end(), vec.begin(), vec.end());
    }
    std::set<std::vector<T>> tmp(res.begin(), res.end()); // remove potential duplicates
    res.assign(tmp.begin(), tmp.end());
    return res;
}

#endif // VEC_UTILS_H
