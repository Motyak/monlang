#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <vector>
#include <string>
#include <sstream>
#include <map>

inline std::string str(char c) {
    static const std::map<char, std::string> invisible_chars = {
        {-1, "EOF"},
        {0, "NUL"},
        {9, "TAB"},
        {10, "NEWLINE(LF)"},
        {32, "SPACE"},
    };
    if (invisible_chars.find(c) != invisible_chars.end()) {
        return invisible_chars.at(c);
    }
    if (c == '`') {
        return "'" + std::string(1, c) + "'";
    }
    return "`" + std::string(1, c) + "`";
}

inline std::string str(std::vector<CharacterAppearance> seq) {
    std::string res("{");
    for (size_t i = 0; i < seq.size() - 1; ++i) {
        res += std::to_string(seq[i].ntimes) + " * ";
        res += str(seq[i]) + ", ";
    }
    if (seq.size() != 0) {
        res += std::to_string(seq[seq.size() - 1].ntimes) + " * ";
        res += str(seq[seq.size() - 1]);
    }
    res += "}";
    return res;
}

inline std::vector<std::string> split(const std::string& input, const std::string& delim) {
    std::vector<std::string> res;
    std::string token = "";
    for (size_t i = 0; i < input.size(); i++) {
        bool flag = true;
        for (size_t j = 0; j < delim.size(); j++) {
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

inline std::string peekRemainingCharacters(std::istringstream& iss) {
    return iss.str().substr(iss.tellg());
}

#endif //STR_UTILS_H
