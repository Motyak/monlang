#include <monlang/ParenthesesGroup.h>
#include <monlang/Term.h>
#include <monlang/common.h>

/* impl only */
#include <monlang/ast/ProgramSentence.h>

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

MayFail<MayFail_<ParenthesesGroup>> consumeParenthesesGroup(std::istringstream& input) {
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

///////////////////////////////////////////////////////////

ParenthesesGroup::ParenthesesGroup(const std::vector<Term>& terms) : terms(terms){}

MayFail_<ParenthesesGroup>::MayFail_(const std::vector<MayFail<MayFail_<Term>>>& terms) : terms(terms){}

MayFail_<ParenthesesGroup>::MayFail_(const ParenthesesGroup& pg) {
    this->terms = vec_cast<MayFail<MayFail_<Term>>>(pg.terms);
    this->_tokenLen = pg._tokenLen;
    this->_tokenId = pg._tokenId;
}

MayFail_<ParenthesesGroup>::operator ParenthesesGroup() const {
    auto ppg = ParenthesesGroup{vec_cast<Term>(terms)};
    ppg._tokenLen = this->_tokenLen;
    ppg._tokenId = this->_tokenId;
    return ppg;
}
