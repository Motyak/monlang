#ifndef ATOM_H
#define ATOM_H

#include <monlang/common.h>

#include <variant>

struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
struct Association;

struct Atom {
    std::string value;
};

MayFail<Atom> consumeAtomStrictly(const std::vector<char>& terminatorCharacters, std::istringstream& input);

using consumeAtom_RetType = std::variant<
    MayFail<Atom*>,
    MayFail<MayFail_<PostfixParenthesesGroup>*>,
    MayFail<MayFail_<PostfixSquareBracketsGroup>*>,
    MayFail<MayFail_<Association>*>
>;
consumeAtom_RetType consumeAtom(std::vector<char> terminatorCharacters, std::istringstream& input);

#endif // ATOM_H
