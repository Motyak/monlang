#include <monlang/MultilineSquareBracketsGroup.h>
#include <monlang/ProgramSentence.h>
#include <monlang/common.h>

/* impl only */
#include <monlang/ast/SquareBracketsGroup.h>

#include <utils/vec-utils.h>
#include <utils/defer-util.h>

#define until(x) while(!(x))

const Sequence MultilineSquareBracketsGroup::INITIATOR_SEQUENCE __attribute__((init_priority(6000))) = vec_concat({
    SquareBracketsGroup::INITIATOR_SEQUENCE,
    ProgramSentence::TERMINATOR_SEQUENCE,
});
const Sequence MultilineSquareBracketsGroup::TERMINATOR_SEQUENCE = SquareBracketsGroup::TERMINATOR_SEQUENCE;

const std::vector<char> MultilineSquareBracketsGroup::RESERVED_CHARACTERS = {
    sequenceFirstChar(INITIATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value(),
};

MayFail<MayFail_<MultilineSquareBracketsGroup>> consumeMultilineSquareBracketsGroup(std::istringstream& input) {
    RECORD_INPUT_STREAM_PROGRESS();
    TRACE_CUR_FUN();
    GLOBAL indentLevel;
    auto indentedTerminatorSeq = vec_concat({INDENT_SEQUENCE(), MultilineSquareBracketsGroup::TERMINATOR_SEQUENCE});

    if (!consumeSequence(MultilineSquareBracketsGroup::INITIATOR_SEQUENCE, input)) {
        return Malformed(MayFail_<MultilineSquareBracketsGroup>{}, ERR(044));
    }

    if (peekSequence(indentedTerminatorSeq, input)) {
        return Malformed(MayFail_<MultilineSquareBracketsGroup>{}, ERR(441));
    }

    indentLevel++;
    defer { indentLevel--; }; // restore indent level, because global

    std::vector<MayFail<MayFail_<ProgramSentence>>> sentences;
    MayFail<MayFail_<ProgramSentence>> currentSentence;

    auto newlines = size_t(0);
    until (input.peek() == EOF || peekSequence(indentedTerminatorSeq, input)) {
        currentSentence = consumeProgramSentence(input);
        if (!currentSentence.has_error() && currentSentence.value().programWords.size() == 0) {
            newlines += currentSentence.value()._tokenIndentSpaces
                    + currentSentence.value()._tokenLen;
            continue; // ignore empty sentences
        }
        currentSentence.val._tokenLeadingNewlines = newlines;
        newlines = 0;
        sentences.push_back(currentSentence);
        if (currentSentence.has_error()) {
            return Malformed(MayFail_<MultilineSquareBracketsGroup>{sentences}, ERR(449));
        }
    }

    if (!consumeSequence(indentedTerminatorSeq, input)) {
        return Malformed(MayFail_<MultilineSquareBracketsGroup>{sentences}, ERR(440));
    }

    if (sentences.size() == 0) {
        return Malformed(MayFail_<MultilineSquareBracketsGroup>{}, ERR(442));
    }

    // add trailing newlines to the latest sentence
    sentences.back().val._tokenTrailingNewlines = newlines;

    auto msbg = MayFail_<MultilineSquareBracketsGroup>{sentences};
    msbg._tokenLen = GET_INPUT_STREAM_PROGRESS();
    return msbg;
}

///////////////////////////////////////////////////////////

MultilineSquareBracketsGroup::MultilineSquareBracketsGroup(const std::vector<ProgramSentence>& sentences)
        : sentences(sentences){}

MayFail_<MultilineSquareBracketsGroup>::MayFail_(const std::vector<MayFail<MayFail_<ProgramSentence>>>& sentences)
        : sentences(sentences){}

MayFail_<MultilineSquareBracketsGroup>::MayFail_(const MultilineSquareBracketsGroup& msbg) {
    auto sentences = std::vector<MayFail<MayFail_<ProgramSentence>>>{};
    for (auto e: msbg.sentences) {
        sentences.push_back(wrap(e));
    }

    this->sentences = sentences;
    this->_tokenLen = msbg._tokenLen;
    this->_tokenId = msbg._tokenId;
}

MayFail_<MultilineSquareBracketsGroup>::operator MultilineSquareBracketsGroup() const {
    auto sentences = std::vector<ProgramSentence>{};
    for (auto e: this->sentences) {
        sentences.push_back(unwrap(e.value()));
    }

    auto msbg = MultilineSquareBracketsGroup{sentences};
    msbg._tokenLen = this->_tokenLen;
    msbg._tokenId = this->_tokenId;
    return msbg;
}
