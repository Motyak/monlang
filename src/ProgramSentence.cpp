#include <monlang/ProgramSentence.h>
#include <monlang/Word.h>
#include <monlang/common.h>

#include <utils/assert-utils.h>
#include <utils/loop-utils.h>

#include <algorithm>

const Sequence ProgramSentence::TAB_SEQUENCE = {{SPACE, 4_}};
const Sequence ProgramSentence::CONTINUATOR_SEQUENCE = {SPACE};
const Sequence ProgramSentence::TERMINATOR_SEQUENCE = {NEWLINE};

const std::vector<char> ProgramSentence::RESERVED_CHARACTERS = {
    sequenceFirstChar(TAB_SEQUENCE).value(),
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value()
};

MayFail<MayFail_<ProgramSentence>> consumeProgramSentence(std::istringstream& input, int indentLevel) {
    TRACE_CUR_FUN();

    if (input.peek() == EOF) {
        return Malformed(MayFail_<ProgramSentence>{}, ERR(125));
    }

    // empty sentences are valid, just discarded by the Program
    if (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(ProgramSentence::TERMINATOR_SEQUENCE));
        return MayFail_<ProgramSentence>{};
    }

    if (indentLevel > 0 && !consumeSequence(INDENT_SEQUENCE(), input)) {
        return Malformed(MayFail_<ProgramSentence>{}, ERR(123));
    }

    // indented empty sentences are valid, just discarded by the Program
    if (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(ProgramSentence::TERMINATOR_SEQUENCE));
        return MayFail_<ProgramSentence>{};
    }

    if (peekSequence(ProgramSentence::CONTINUATOR_SEQUENCE, input)) {
        return Malformed(MayFail_<ProgramSentence>{}, ERR(121));
    }

    std::vector<MayFail<ProgramWord_>> programWords;
    MayFail<ProgramWord_> currentWord;

    LOOP until (input.peek() == EOF || peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
    if (!__first_it)
    {
        if (!consumeSequence(ProgramSentence::CONTINUATOR_SEQUENCE, input)) {
            // this happens when we have an Atom right after a non-Atom (without a space in between)
            return Malformed(MayFail_<ProgramSentence>{programWords}, ERR(102));
        }
        if (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
            return Malformed(MayFail_<ProgramSentence>{programWords}, ERR(122));
        }
    }
        currentWord = consumeProgramWord(input);
        programWords.push_back(currentWord);
        if (currentWord.has_error()) {
            return Malformed(MayFail_<ProgramSentence>{programWords}, ERR(129));
        }

        ENDLOOP
    }

    if (!consumeSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        return Malformed(MayFail_<ProgramSentence>{programWords}, ERR(120));
    }

    // eat and discard trailing newlines as well
    while (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(ProgramSentence::TERMINATOR_SEQUENCE));
    }

    return MayFail_<ProgramSentence>{programWords};
}

///////////////////////////////////////////////////////////

MayFail_<ProgramSentence> ProgramSentence::wrap() const {
    std::vector<MayFail<ProgramWord_>> programWords;
    for (auto e: this->programWords) {
        programWords.push_back(wrap_pw(e));
    }
    return MayFail_<ProgramSentence>{programWords};
}

MayFail_<ProgramSentence>::MayFail_(std::vector<MayFail<ProgramWord_>> programWords) : programWords(programWords){}

MayFail_<ProgramSentence>::MayFail_(ProgramSentence sentence) {
    *this = sentence.wrap();
}

MayFail_<ProgramSentence>::operator ProgramSentence() const {
    std::vector<ProgramWord> res;
    for (auto e: programWords) {
        res.push_back(unwrap_pw(e.value()));
    }
    return ProgramSentence{res};
}

ProgramSentence MayFail_<ProgramSentence>::unwrap() const {
    return (ProgramSentence)*this;
}
