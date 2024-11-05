#include <monlang/Atom.h>
#include <monlang/common.h>

/* in impl only */
#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>

#include <utils/loop-utils.h>

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

    /* look behind */

    using PostfixLeftPart = std::variant<Atom*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = move_to_heap(atom.value());

    for (;;) {
        #ifndef DISABLE_PPG_IN_ATOM
        if (auto whats_right_behind = tryConsumePostfixParenthesesGroup(&accumulatedPostfixLeftPart, input)) {
            if (!whats_right_behind->has_value()) {
                return *whats_right_behind; // malformed postfix
            }
            continue;
        }
        #endif

        #ifndef DISABLE_PSBG_IN_ATOM
        if (auto whats_right_behind = tryConsumePostfixSquareBracketsGroup(&accumulatedPostfixLeftPart, input)) {
            if (!whats_right_behind->has_value()) {
                return *whats_right_behind; // malformed postfix
            }
            continue;
        }
        #endif

        break;
    }

    return std::visit(
        [](auto word) -> consumeAtom_RetType {return word;},
        accumulatedPostfixLeftPart
    );
}
