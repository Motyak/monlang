#ifndef AST_ATOM_H
#define AST_ATOM_H

#include <string>

struct Atom {
    std::string value;

    size_t _tokenLen = 0;
    size_t _tokenId = 123456789;
    Atom() = default;
    Atom(const std::string&);
};

#endif // AST_ATOM_H
