#include <monlang/ParenthesesGroup.h>
#include <monlang/Term.h>
#include <monlang/common.h>

/* impl only */
#include <monlang/ast/ProgramSentence.h>
#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>
#include <monlang/Association.h>

#include <utils/loop-utils.h>
#include <utils/variant-utils.h>
#include <utils/vec-utils.h>

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
    RECORD_INPUT_STREAM_PROGRESS();
    std::vector<char> terminatorCharacters = {
        sequenceFirstChar(ParenthesesGroup::TERMINATOR_SEQUENCE).value(),
        sequenceFirstChar(ProgramSentence::TERMINATOR_SEQUENCE).value(),
    };
    const std::vector<char> termTerminatorChars = {
        sequenceFirstChar(ParenthesesGroup::CONTINUATOR_SEQUENCE).value(),
        sequenceFirstChar(ParenthesesGroup::TERMINATOR_SEQUENCE).value(),
        sequenceFirstChar(ProgramSentence::TERMINATOR_SEQUENCE).value(),
    };

    if (!consumeSequence(ParenthesesGroup::INITIATOR_SEQUENCE, input)) {
        return Malformed(MayFail_<ParenthesesGroup>{}, ERR(042));
    }

    if (peekSequence(ParenthesesGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(ParenthesesGroup::TERMINATOR_SEQUENCE));
        auto empty_pg = MayFail_<ParenthesesGroup>{};
        empty_pg._tokenLen = GET_INPUT_STREAM_PROGRESS();
        return empty_pg;
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

    auto pg = MayFail_<ParenthesesGroup>{terms};
    pg._tokenLen = GET_INPUT_STREAM_PROGRESS();
    return pg;
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

    #ifndef DISABLE_ASSOC_IN_PG
    if (peekSequence(Association::SEPARATOR_SEQUENCE, input)) {
        return consumeAssociation(accumulatedPostfixLeftPart, input); /*
            early return assoc (malformed or not).
            Association can contain a PostfixLeftPart..
            .., but not the other way around! (precedence rule)
        */
    }
    #endif

    return std::visit(
        [](auto word) -> consumeParenthesesGroup_RetType {return move_to_heap(wrap(*word));},
        accumulatedPostfixLeftPart
    );
}

///////////////////////////////////////////////////////////

ParenthesesGroup::ParenthesesGroup(const std::vector<Term>& terms) : terms(terms){}

MayFail_<ParenthesesGroup>::MayFail_(const std::vector<MayFail<MayFail_<Term>>>& terms) : terms(terms){}

MayFail_<ParenthesesGroup>::MayFail_(const ParenthesesGroup& pg) {
    this->terms = vec_cast<MayFail<MayFail_<Term>>>(pg.terms);
    this->_tokenLen = pg._tokenLen;
}

MayFail_<ParenthesesGroup>::operator ParenthesesGroup() const {
    auto ppg = ParenthesesGroup{vec_cast<Term>(terms)};
    ppg._tokenLen = this->_tokenLen;
    return ppg;
}
