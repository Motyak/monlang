#include <monlang/SquareBracketsGroup.h>
#include <monlang/Term.h>
#include <monlang/common.h>

/* impl only */
#include <monlang/ast/ProgramSentence.h>

#include <utils/loop-utils.h>
#include <utils/variant-utils.h>
#include <utils/vec-utils.h>

const Sequence SquareBracketsGroup::INITIATOR_SEQUENCE __attribute__((init_priority(3000))) = {'['};
const Sequence SquareBracketsGroup::CONTINUATOR_SEQUENCE = {',', SPACE};
const Sequence SquareBracketsGroup::TERMINATOR_SEQUENCE __attribute__((init_priority(3000))) = {']'};

const std::vector<char> SquareBracketsGroup::RESERVED_CHARACTERS = {
    sequenceFirstChar(INITIATOR_SEQUENCE).value(),
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value(),
};

MayFail<MayFail_<SquareBracketsGroup>> consumeSquareBracketsGroup(std::istringstream& input) {
    RECORD_INPUT_STREAM_PROGRESS();
    TRACE_CUR_FUN();
    const std::vector<char> terminatorCharacters = {
        sequenceFirstChar(SquareBracketsGroup::TERMINATOR_SEQUENCE).value(),
        sequenceFirstChar(ProgramSentence::TERMINATOR_SEQUENCE).value(),
    };
    const std::vector<char> termTerminatorChars = {
        sequenceFirstChar(SquareBracketsGroup::CONTINUATOR_SEQUENCE).value(),
        sequenceFirstChar(SquareBracketsGroup::TERMINATOR_SEQUENCE).value(),
        sequenceFirstChar(ProgramSentence::TERMINATOR_SEQUENCE).value(),
    };

    if (!consumeSequence(SquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return Malformed(MayFail_<SquareBracketsGroup>{}, ERR(043));
    }

    if (peekSequence(SquareBracketsGroup::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(SquareBracketsGroup::TERMINATOR_SEQUENCE));
        auto empty_sbg = MayFail_<SquareBracketsGroup>{};
        empty_sbg._tokenLen = GET_INPUT_STREAM_PROGRESS();
        return empty_sbg;
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

    auto sbg = MayFail_<SquareBracketsGroup>{terms};
    sbg._tokenLen = GET_INPUT_STREAM_PROGRESS();
    return sbg;
}

///////////////////////////////////////////////////////////

SquareBracketsGroup::SquareBracketsGroup(const std::vector<Term>& terms) : terms(terms){}

MayFail_<SquareBracketsGroup>::MayFail_(const std::vector<MayFail<MayFail_<Term>>>& terms) : terms(terms){}

MayFail_<SquareBracketsGroup>::MayFail_(const SquareBracketsGroup& sbg) {
    this->terms = vec_cast<MayFail<MayFail_<Term>>>(sbg.terms);
    this->_tokenLen = sbg._tokenLen;
    this->_tokenId = sbg._tokenId;
}

MayFail_<SquareBracketsGroup>::operator SquareBracketsGroup() const {
    auto sbg = SquareBracketsGroup{vec_cast<Term>(terms)};
    sbg._tokenLen = this->_tokenLen;
    sbg._tokenId = this->_tokenId;
    return sbg;
}
