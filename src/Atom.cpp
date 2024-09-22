#include <monlang/Atom.h>
#include <monlang/common.h>

/* in impl only */
#include <monlang/ParenthesesGroup.h>
#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>

#include <utils/mem-utils.h>
#include <utils/variant-utils.h>

#include <algorithm>

#define until(x) while(!(x))

MayFail<Atom> consumeAtomStrictly(const std::vector<char>& terminatorCharacters, std::istringstream& input) {
    TRACE_CUR_FUN();

    if (input.peek() == EOF) {
        return std::unexpected(Malformed(Atom{}, Error{991}));
    }

    std::string value;
    char currentChar;
    until (input.peek() == EOF || std::any_of(
            terminatorCharacters.begin(),
            terminatorCharacters.end(),
            [&input](auto terminatorChar){return input.peek() == terminatorChar;})) {
        input.get(currentChar);
        value += currentChar;
    }

    // means we hit a reserved character
    if (value.size() == 0) {
        return std::unexpected(Malformed(Atom{}, Error{992}));
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

    BEGIN:
    if (peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        auto whats_right_behind = consumeSquareBracketsGroupStrictly(input);
        auto curr_psbg = move_to_heap(PostfixSquareBracketsGroup{
            variant_cast(accumulatedPostfixLeftPart),
            whats_right_behind
        });
        if (!whats_right_behind.has_value()) {
            return std::unexpected(Malformed(curr_psbg, Error{329}));
        }
        accumulatedPostfixLeftPart = curr_psbg;
        goto BEGIN;
    }
    if (peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        auto whats_right_behind = consumeParenthesesGroupStrictly(input);
        auto curr_ppg = move_to_heap(PostfixParenthesesGroup{
            variant_cast(accumulatedPostfixLeftPart),
            whats_right_behind
        });
        if (!whats_right_behind.has_value()) {
            return std::unexpected(Malformed(curr_ppg, Error{319}));
        }
        accumulatedPostfixLeftPart = curr_ppg;
        goto BEGIN;
    }

    return std::visit(overload{
        [](Atom* atom) -> consumeAtom_RetType {return atom;},
        [](PostfixSquareBracketsGroup* psbg) -> consumeAtom_RetType {return psbg;},
        [](PostfixParenthesesGroup* ppg) -> consumeAtom_RetType {return ppg;}
    }, accumulatedPostfixLeftPart);
}
