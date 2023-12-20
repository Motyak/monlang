#ifndef ATOM_H
#define ATOM_H

#include <string>
#include <optional>
#include <variant>
#include <sstream>

struct Atom {
    std::string value;
};

struct Association;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
std::variant<Atom, Association*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*> consumeAtom(std::istringstream&);

Atom consumeAtomStrictly(std::istringstream&);

#endif // ATOM_H
