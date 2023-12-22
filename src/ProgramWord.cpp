#include <ProgramWord.h>
#include <SquareBracketsGroup.h>
#include <ParenthesesGroup.h>
#include <Atom.h>
#include <utils/variant-utils.h>

#include <iostream>

ProgramWord consumeProgramWord(std::istringstream& input) {
    if (auto potentialParenthesesGroup = tryConsumeParenthesesGroup(input)) {
        return variant_cast(*potentialParenthesesGroup);
    }
    if (auto potentialSquareBracketsGroup = tryConsumeSquareBracketsGroup(input)) {
        return variant_cast(*potentialSquareBracketsGroup);
    }
    // if (auto potentialCurlyBracketsGroup = tryConsumeCurlyBracketsGroup(input)) {
    //     return *potentialCurlyBracketsGroup;
    // }
    // if (auto potentialQuotation = tryConsumeQuotation(input)) {
    //     return *potentialQuotation;
    // }

    std::variant<Atom, Association*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*> atom;
    try {
        atom = consumeAtom(input);
    } catch (std::runtime_error& e) {
        std::cerr << "expected a word" << std::endl;
        throw std::runtime_error("user exception");
    };
    return variant_cast(atom);
}
