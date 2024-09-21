#ifndef ATOM_H
#define ATOM_H

#include <monlang/PostfixSquareBracketsGroup.h>
#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/common.h>

struct Atom {
    std::string value;
};

MayFail<Atom> consumeAtomStrictly(const std::vector<char>& terminatorCharacters, std::istringstream& input);

using consumeAtom_RetType = std::variant<
    MayFail<Atom>,
    MayFail<PostfixSquareBracketsGroup>,
    MayFail<PostfixParenthesesGroup>
>;
consumeAtom_RetType consumeAtom(const std::vector<char>& terminatorCharacters, std::istringstream& input);

#endif // ATOM_H
