#ifndef AST_QUOTATION_H
#define AST_QUOTATION_H

#include <monlang/ast/common.h>

#include <string>

struct Quotation {
    static const Sequence DELIMITERS_SEQUENCE;
    static const Sequence ALT_DELIMITERS_SEQUENCE; // multiline
    static const std::vector<char> SINGLELINE_RESERVED_CHARACTERS;
    static const std::vector<char> MULTILINE_RESERVED_CHARACTERS;

    std::string quoted;

    size_t _tokenLen = 0;
    bool _multiline = false;

    Quotation() = default;
    Quotation(const std::string&);
};

#endif // AST_QUOTATION_H
