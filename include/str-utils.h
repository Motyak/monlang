#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <vector>
#include <string>

std::vector<std::string> split(const std::string& input, const std::string& delim) {
    std::vector<std::string> res;
    std::string token = "";
    for (int i = 0; i < input.size(); i++) {
        bool flag = true;
        for (int j = 0; j < delim.size(); j++) {
            if (input[i + j] != delim[j]) {
                flag = false;
            }
        }
        if (flag) {
            if (token.size() > 0) {
                res.push_back(token);
                token = "";
                i += delim.size() - 1;
            }
        } else {
            token += input[i];
        }
    }
    res.push_back(token);
    return res;
}

#endif //STR_UTILS_H
