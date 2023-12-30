#ifndef ATOM_H
#define ATOM_H

#include <common.h>

#include <variant>
#include <sstream>

struct Atom {
    std::string value;
};

struct Association;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
MayFail<std::variant<Atom, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>> consumeAtom(std::istringstream&);

MayFail<Atom> consumeAtomStrictly(std::istringstream&);

#endif // ATOM_H
