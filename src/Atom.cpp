#include <monlang/Atom.h>
#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/common.h>

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
        return atom;
    }

    using PostfixLeftPart = std::variant<Atom*, PostfixParenthesesGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = move_to_heap(atom.value());

    BEGIN:
    if (peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        auto whats_right_behind = consumeParenthesesGroupStrictly(input);
        auto curr_ppg = PostfixParenthesesGroup{
            variant_cast(accumulatedPostfixLeftPart),
            whats_right_behind
        };
        if (!whats_right_behind.has_value()) {
            return std::unexpected(Malformed(curr_ppg, Error{319}));
        }
        accumulatedPostfixLeftPart = move_to_heap(curr_ppg);
        goto BEGIN;
    }

    return std::visit(overload{
        [](Atom* atom) -> consumeAtom_RetType {return *atom;},
        [](PostfixParenthesesGroup* ppg) -> consumeAtom_RetType {return *ppg;}
    }, accumulatedPostfixLeftPart);
}
