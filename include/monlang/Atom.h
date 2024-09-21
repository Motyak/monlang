#ifndef ATOM_H
#define ATOM_H

#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/common.h>

struct Atom {
    std::string value;
};

MayFail<Atom>
consumeAtomStrictly(const std::vector<char>& terminatorCharacters, std::istringstream& input);

std::variant<MayFail<Atom>, MayFail<PostfixParenthesesGroup>>
consumeAtom(const std::vector<char>& terminatorCharacters, std::istringstream& input);

#endif // ATOM_H
