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

MayFail<MayFail_<ParenthesesGroup>> consumeParenthesesGroupStrictly(std::istringstream& input) {
    TRACE_CUR_FUN();
    std::vector<char> terminatorCharacters = {
        sequenceFirstChar(ParenthesesGroup::TERMINATOR_SEQUENCE).value()
    };
    const std::vector<char> termTerminatorChars = {
        sequenceFirstChar(ParenthesesGroup::CONTINUATOR_SEQUENCE).value(),
        sequenceFirstChar(ParenthesesGroup::TERMINATOR_SEQUENCE).value()
    };

    if (!consumeSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        return Malformed(MayFail_<ParenthesesGroup>{}, ERR(042));
    }

    if (peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(ParenthesesGroup::TERMINATOR_SEQUENCE));
        return MayFail_<ParenthesesGroup>{};
    }

    std::vector<MayFail<MayFail_<Term>>> terms;
    MayFail<MayFail_<Term>> currentTerm;

    LOOP until (input.peek() == EOF || peekAnyChar(terminatorCharacters, input)) {
    if (!__first_it)
    {
        if (!consumeSequence(ParenthesesGroup::CONTINUATOR_SEQUENCE, input)) {
            return Malformed(MayFail_<ParenthesesGroup>{terms}, ERR(402));
        }
    }
        currentTerm = consumeTerm(termTerminatorChars, input);
        terms.push_back(currentTerm);
        if (currentTerm.has_error()) {
            return Malformed(MayFail_<ParenthesesGroup>{terms}, ERR(429));
        }

        ENDLOOP
    }

    if (!consumeSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        return Malformed(MayFail_<ParenthesesGroup>{terms}, ERR(420));
    }

    return MayFail_<ParenthesesGroup>{terms};
}

consumeParenthesesGroup_RetType consumeParenthesesGroup(std::istringstream& input) {
    auto pg = consumeParenthesesGroupStrictly(input);

    if (pg.has_error()) {
        return mayfail_convert<MayFail_<ParenthesesGroup>*>(pg);
    }

    /* look behind */

    using PostfixLeftPart = std::variant<ParenthesesGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>;
    PostfixLeftPart accumulatedPostfixLeftPart = move_to_heap((ParenthesesGroup)pg);

    for (;;) {
        #ifndef DISABLE_PPG_IN_PG
        if (peekSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
            auto ppg = consumePostfixParenthesesGroup(&accumulatedPostfixLeftPart, input);
            if (ppg.has_error()) {
                return ppg; // malformed postfix
            }
            continue;
        }
        #endif

        #ifndef DISABLE_PSBG_IN_PG
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
        [](auto word) -> consumeParenthesesGroup_RetType {return move_to_heap(word->wrap());},
        accumulatedPostfixLeftPart
    );
}

///////////////////////////////////////////////////////////

MayFail_<ParenthesesGroup> ParenthesesGroup::wrap() const {
    return MayFail_<ParenthesesGroup>{vec_cast<MayFail<MayFail_<Term>>>(this->terms)};
}

MayFail_<ParenthesesGroup>::MayFail_(std::vector<MayFail<MayFail_<Term>>> terms) : terms(terms){}

MayFail_<ParenthesesGroup>::MayFail_(ParenthesesGroup pg) : MayFail_(pg.wrap()){}

MayFail_<ParenthesesGroup>::operator ParenthesesGroup() const {
    return ParenthesesGroup{vec_cast<Term>(terms)};
}

ParenthesesGroup MayFail_<ParenthesesGroup>::unwrap() const {
    return (ParenthesesGroup)*this;
}
