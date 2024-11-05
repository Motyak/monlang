#include <monlang/ParenthesesGroup.h>
#include <monlang/Term.h>
#include <monlang/common.h>

/* impl only */
#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>

#include <utils/loop-utils.h>
#include <utils/variant-utils.h>

#include <algorithm>

const Sequence ParenthesesGroup::INITIATOR_SEQUENCE = {'('};
const Sequence ParenthesesGroup::CONTINUATOR_SEQUENCE = {',', SPACE};
const Sequence ParenthesesGroup::TERMINATOR_SEQUENCE = {')'};

const std::vector<char> ParenthesesGroup::RESERVED_CHARACTERS = {
    sequenceFirstChar(INITIATOR_SEQUENCE).value(),
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value(),
};

MayFail<ParenthesesGroup> consumeParenthesesGroupStrictly(std::istringstream& input) {
    TRACE_CUR_FUN();
    std::vector<char> terminatorCharacters = {
        sequenceFirstChar(ParenthesesGroup::TERMINATOR_SEQUENCE).value()
    };
    const std::vector<char> termTerminatorChars = {
        sequenceFirstChar(ParenthesesGroup::CONTINUATOR_SEQUENCE).value(),
        sequenceFirstChar(ParenthesesGroup::TERMINATOR_SEQUENCE).value()
    };

    if (!consumeSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ParenthesesGroup{}, ERR(042)));
    }

    if (peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(ParenthesesGroup::TERMINATOR_SEQUENCE));
        return ParenthesesGroup{};
    }

    std::vector<MayFail<Term>> terms;
    MayFail<Term> currentTerm;

    LOOP until (input.peek() == EOF || peekAnyChar(terminatorCharacters, input)) {
    if (!__first_it)
    {
        if (!consumeSequence(ParenthesesGroup::CONTINUATOR_SEQUENCE, input)) {
            return std::unexpected(Malformed(ParenthesesGroup{terms}, ERR(402)));
        }
    }
        currentTerm = consumeTerm(termTerminatorChars, input);
        terms.push_back(currentTerm);
        if (!currentTerm.has_value()) {
            return std::unexpected(Malformed(ParenthesesGroup{terms}, ERR(429)));
        }

        ENDLOOP
    }

    if (!consumeSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ParenthesesGroup{terms}, ERR(420)));
    }

    return ParenthesesGroup{terms};
}

consumeParenthesesGroup_RetType consumeParenthesesGroup(std::istringstream& input) {
    auto pg = consumeParenthesesGroupStrictly(input);

    if (!pg.has_value()) {
        return mayfail_convert<ParenthesesGroup*>(pg);
    }

    /* look behind */

    using PostfixLeftPart = std::variant<ParenthesesGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = move_to_heap(pg.value());

    for (;;) {
        #ifndef DISABLE_PPG_IN_PG
        if (auto whats_right_behind = tryConsumePostfixParenthesesGroup(&accumulatedPostfixLeftPart, input)) {
            if (!whats_right_behind->has_value()) {
                return *whats_right_behind; // malformed postfix
            }
            continue;
        }
        #endif

        #ifndef DISABLE_PSBG_IN_PG
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
        [](auto word) -> consumeParenthesesGroup_RetType {return word;},
        accumulatedPostfixLeftPart
    );
}
