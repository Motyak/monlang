#include <ProgramWord.h>
#include <SquareBracketsGroup.h>
#include <ParenthesesGroup.h>
#include <CurlyBracketsGroup.h>
#include <Atom.h>
#include <utils/variant-utils.h>

#include <iostream>

MayFail<ProgramWord> consumeProgramWord(std::istringstream& input) {
    if (auto potentialParenthesesGroup = tryConsumeParenthesesGroup(input)) {
        if (!*potentialParenthesesGroup) {
            return std::unexpected(Error{110, new Error{potentialParenthesesGroup->error()}});
        }
        return variant_cast(**potentialParenthesesGroup);
    }
    if (auto potentialSquareBracketsGroup = tryConsumeSquareBracketsGroup(input)) {
        if (!*potentialSquareBracketsGroup) {
            return std::unexpected(Error{111, new Error{potentialSquareBracketsGroup->error()}});
        }
        return variant_cast(**potentialSquareBracketsGroup);
    }
    // if (auto potentialCurlyBracketsGroup = tryConsumeCurlyBracketsGroup(input)) {
    //     if (!*potentialCurlyBracketsGroup) {
    //         return std::unexpected(Error{112, new Error{potentialCurlyBracketsGroup->error()}});
    //     }
    //     return variant_cast(**potentialCurlyBracketsGroup);
    // }
    // if (auto potentialQuotation = tryConsumeQuotation(input)) {
    //     if (!*potentialQuotation) {
    //         return std::unexpected(Error{113, new Error{potentialQuotation->error()}});
    //     }
    //     return variant_cast(**potentialQuotation);
    // }
    
    if (auto atom = consumeAtom(input); !atom) {
        return std::unexpected(Error{114, new Error{atom.error()}});
    } else {
        return variant_cast(*atom);
    }
}
