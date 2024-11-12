#include <monlang/Atom.h>
#include <monlang/common.h>

/* in impl only */
#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>
#include <monlang/Association.h>

#include <utils/loop-utils.h>
#include <utils/vec-utils.h>

MayFail<Atom> consumeAtomStrictly(const std::vector<char>& terminatorCharacters, std::istringstream& input) {
    TRACE_CUR_FUN();

    if (input.peek() == EOF) {
        return Malformed(Atom{}, ERR(995));
    }

    std::string value;
    char currentChar;

    until (input.peek() == EOF || peekAnyChar(terminatorCharacters, input)) {
        input.get(currentChar);
        value += currentChar;
    }

    // means we hit a reserved character
    if (value.size() == 0) {
        return Malformed(Atom{}, ERR(992));
    }

    return Atom{value};
}

consumeAtom_RetType consumeAtom(std::vector<char> terminatorCharacters, std::istringstream& input) {
    #ifndef DISABLE_ASSOC_IN_ATOM
    terminatorCharacters = vec_union({
        terminatorCharacters,
        Association::RESERVED_CHARACTERS
    });
    #endif
    auto atom = consumeAtomStrictly(terminatorCharacters, input);

    if (atom.has_error()) {
        return mayfail_convert<Atom*>(atom);
    }

    /* look behind */

    using PostfixLeftPart = std::variant<Atom*, MayFail_<PostfixParenthesesGroup>*, MayFail_<PostfixSquareBracketsGroup>*>;
    PostfixLeftPart accumulatedPostfixLeftPart = move_to_heap(atom.value());

    for (;;) {
        #ifndef DISABLE_PPG_IN_ATOM
        if (peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
            auto ppg = consumePostfixParenthesesGroup(&accumulatedPostfixLeftPart, input);
            if (ppg.has_error()) {
                return ppg; // malformed postfix
            }
            continue;
        }
        #endif

        #ifndef DISABLE_PSBG_IN_ATOM
        if (peekSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
            auto psbg = consumePostfixSquareBracketsGroup(&accumulatedPostfixLeftPart, input);
            if (psbg.has_error()) {
                return psbg; // malformed postfix
            }
            continue;
        }
        #endif

        break;
    }

    #ifndef DISABLE_ASSOC_IN_ATOM
    if (peekSequence(Association::SEPARATOR_SEQUENCE, input)) {
        return consumeAssociation(accumulatedPostfixLeftPart, input); /*
            early return assoc (malformed or not).
            Association can contain a PostfixLeftPart..
            .., but not the other way around! (precedence rule)
        */
    }
    #endif

    return std::visit(
        [](auto word) -> consumeAtom_RetType {return word;},
        accumulatedPostfixLeftPart
    );
}
