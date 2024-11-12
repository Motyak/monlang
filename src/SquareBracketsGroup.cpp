#include <monlang/SquareBracketsGroup.h>
#include <monlang/Term.h>
#include <monlang/common.h>

/* impl only */
#include <monlang/PostfixSquareBracketsGroup.h>

#include <utils/loop-utils.h>
#include <utils/variant-utils.h>

#include <algorithm>

const Sequence SquareBracketsGroup::INITIATOR_SEQUENCE = {'[', {SPACE, 0_}};
const Sequence SquareBracketsGroup::CONTINUATOR_SEQUENCE = {',', SPACE};
const Sequence SquareBracketsGroup::TERMINATOR_SEQUENCE = {']'};

const std::vector<char> SquareBracketsGroup::RESERVED_CHARACTERS = {
    sequenceFirstChar(INITIATOR_SEQUENCE).value(),
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value(),
};

MayFail<MayFail_<SquareBracketsGroup>> consumeSquareBracketsGroupStrictly(std::istringstream& input) {
    TRACE_CUR_FUN();
    const std::vector<char> terminatorCharacters = {
        sequenceFirstChar(SquareBracketsGroup::TERMINATOR_SEQUENCE).value()
    };
    const std::vector<char> termTerminatorChars = {
        sequenceFirstChar(SquareBracketsGroup::CONTINUATOR_SEQUENCE).value(),
        sequenceFirstChar(SquareBracketsGroup::TERMINATOR_SEQUENCE).value()
    };

    if (!consumeSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return Malformed(MayFail_<SquareBracketsGroup>{}, ERR(043));
    }

    if (peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(SquareBracketsGroup::TERMINATOR_SEQUENCE));
        return MayFail_<SquareBracketsGroup>{};
    }

    std::vector<MayFail<MayFail_<Term>>> terms;
    MayFail<MayFail_<Term>> currentTerm;

    LOOP until (input.peek() == EOF || peekAnyChar(terminatorCharacters, input)) {
    if (!__first_it)
    {
        if (!consumeSequence(SquareBracketsGroup::CONTINUATOR_SEQUENCE, input)) {
            return Malformed(MayFail_<SquareBracketsGroup>{terms}, ERR(403));
        }
    }
        currentTerm = consumeTerm(termTerminatorChars, input);
        terms.push_back(currentTerm);
        if (currentTerm.has_error()) {
            return Malformed(MayFail_<SquareBracketsGroup>{terms}, ERR(439));
        }

        ENDLOOP
    }

    if (!consumeSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        return Malformed(MayFail_<SquareBracketsGroup>{terms}, ERR(430));
    }

    return MayFail_<SquareBracketsGroup>{terms};
}

consumeSquareBracketsGroup_RetType consumeSquareBracketsGroup(std::istringstream& input) {
    auto sbg = consumeSquareBracketsGroupStrictly(input);

    if (sbg.has_error()) {
        return mayfail_convert<MayFail_<SquareBracketsGroup>*>(sbg);
    }

    /* look behind */

    using PostfixLeftPart = std::variant<MayFail_<SquareBracketsGroup>*, MayFail_<PostfixSquareBracketsGroup>*>;
    PostfixLeftPart accumulatedPostfixLeftPart = move_to_heap(sbg.value());

    for (;;) {
        #ifndef DISABLE_PSBG_IN_SBG
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

    return std::visit(
        [](auto word) -> consumeSquareBracketsGroup_RetType {return word;},
        accumulatedPostfixLeftPart
    );
}
