#define ATOM_CPP

#include <monlang/Atom.h>
#include <monlang/common.h>

/* in impl only */
#include <monlang/ParenthesesGroup.h>
#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>

#include <utils/mem-utils.h>
#include <utils/variant-utils.h>
#include <utils/loop-utils.h>

#include <algorithm>

#if __has_include ("mayfail.hpp")
    // enable extern explicit instanciations..
    // ..for common.h 'mayfail' templates
    #include "mayfail.hpp"
    #include "mayfail.tpp"
#endif

MayFail<Atom> consumeAtomStrictly(const std::vector<char>& terminatorCharacters, std::istringstream& input) {
    TRACE_CUR_FUN();

    if (input.peek() == EOF) {
        return std::unexpected(Malformed(Atom{}, ERR(995)));
    }

    std::string value;
    char currentChar;

    until (input.peek() == EOF || peekAnyChar(terminatorCharacters, input)) {
        input.get(currentChar);
        value += currentChar;
    }

    // means we hit a reserved character
    if (value.size() == 0) {
        return std::unexpected(Malformed(Atom{}, ERR(992)));
    }

    return Atom{value};
}

consumeAtom_RetType consumeAtom(const std::vector<char>& terminatorCharacters, std::istringstream& input) {
    auto atom = consumeAtomStrictly(terminatorCharacters, input);

    if (!atom.has_value()) {
        return mayfail_convert<Atom*>(atom);
    }

    using PostfixLeftPart = std::variant<Atom*, PostfixSquareBracketsGroup*, PostfixParenthesesGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = move_to_heap(atom.value());

    [[maybe_unused]]
    BEGIN:
    #ifndef DISABLE_PPG_IN_ATOM
    if (peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        auto whats_right_behind = consumeParenthesesGroupStrictly(input);
        auto curr_ppg = move_to_heap(PostfixParenthesesGroup{
            variant_cast(accumulatedPostfixLeftPart),
            whats_right_behind
        });
        if (!whats_right_behind.has_value()) {
            return std::unexpected(Malformed(curr_ppg, ERR(319)));
        }
        accumulatedPostfixLeftPart = curr_ppg;
        goto BEGIN;
    }
#endif

    #ifndef DISABLE_PSBG_IN_ATOM
    if (peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        auto whats_right_behind = consumeSquareBracketsGroupStrictly(input);
        auto curr_psbg = move_to_heap(PostfixSquareBracketsGroup{
            variant_cast(accumulatedPostfixLeftPart),
            whats_right_behind
        });
        if (!whats_right_behind.has_value()) {
            return std::unexpected(Malformed(curr_psbg, ERR(329)));
        }
        accumulatedPostfixLeftPart = curr_psbg;
        goto BEGIN;
    }
#endif

    return std::visit(
        [](auto word) -> consumeAtom_RetType {return word;},
        accumulatedPostfixLeftPart
    );
}
