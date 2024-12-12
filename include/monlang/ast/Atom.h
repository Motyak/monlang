#ifndef AST_ATOM_H
#define AST_ATOM_H

#include <string>

struct Atom {
    std::string value;

    size_t _tokenLen = 0;
    Atom() = default;
    Atom(const std::string&);
};

#endif // AST_ATOM_H
