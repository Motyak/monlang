#include <ProgramWord.h>
#include <SquareBracketsGroup.h>
#include <Atom.h>
#include <utils/variant-utils.h>

ProgramWord consumeProgramWord(std::istringstream& input) {
    // if (auto potentialParenthesesGroup = tryConsumeParenthesesGroup(input)) {
    //     return *potentialParenthesesGroup;
    // }
    if (auto potentialSquareBracketsGroup = tryConsumeSquareBracketsGroup(input)) {
        return variant_cast(*potentialSquareBracketsGroup);
    }
    // if (auto potentialCurlyBracketsGroup = tryConsumeCurlyBracketsGroup(input)) {
    //     return *potentialCurlyBracketsGroup;
    // }
    // if (auto potentialQuotation = tryConsumeQuotation(input)) {
    //     return *potentialQuotation;
    // }
    return variant_cast(consumeAtom(input));
}
