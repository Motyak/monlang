#ifndef ATOM_H
#define ATOM_H

#include <monlang/ast/Atom.h>

#include <monlang/common.h>

struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
struct Association;

MayFail<Atom> consumeAtom(const std::vector<char>& terminatorCharacters, std::istringstream& input);

#endif // ATOM_H
