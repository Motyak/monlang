#include <monlang/ProgramSentence.h>
#include <monlang/Word.h>
#include <monlang/common.h>

/* required for ProgramSentence(Term) and operator Term() */
#include <monlang/Term.h>

#include <utils/assert-utils.h>
#include <utils/loop-utils.h>
#include <utils/variant-utils.h>

#include <algorithm>

#define SET_MALFORMED_TOKEN_FIELDS(malformed) \
    malformed.val._tokenIndentSpaces = sequenceLen(INDENT_SEQUENCE())

const Sequence ProgramSentence::TAB_SEQUENCE = {{SPACE, 4_}};
const Sequence ProgramSentence::CONTINUATOR_SEQUENCE = {SPACE};
const Sequence ProgramSentence::TERMINATOR_SEQUENCE __attribute__((init_priority(3000))) = {NEWLINE};

const std::vector<char> ProgramSentence::RESERVED_CHARACTERS = {
    sequenceFirstChar(TAB_SEQUENCE).value(),
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value()
};

thread_local int indentLevel = 0;

Sequence INDENT_SEQUENCE() {
    return indentLevel * ProgramSentence::TAB_SEQUENCE;
}

MayFail<MayFail_<ProgramSentence>> consumeProgramSentence(std::istringstream& input) {
    RECORD_INPUT_STREAM_PROGRESS();
    TRACE_CUR_FUN();
    GLOBAL indentLevel;

    if (input.peek() == EOF) {
        return Malformed(MayFail_<ProgramSentence>{}, ERR(125));
    }

    // empty sentences are valid, just discarded by the Program
    if (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(ProgramSentence::TERMINATOR_SEQUENCE));
        auto empty_sentence = MayFail_<ProgramSentence>{};
        empty_sentence._tokenIndentSpaces = GET_INPUT_STREAM_PROGRESS()
                - sequenceLen(ProgramSentence::TERMINATOR_SEQUENCE); // must do this because non-zero indentLevel..
                                                                     // ..doesn't necessarily mean non-zero indent spaces
        empty_sentence._tokenLen = GET_INPUT_STREAM_PROGRESS();
        return empty_sentence;
    }

    if (indentLevel > 0 && !consumeSequence(INDENT_SEQUENCE(), input)) {
        return Malformed(MayFail_<ProgramSentence>{}, ERR(123));
    }

    if (peekSequence(ProgramSentence::CONTINUATOR_SEQUENCE, input)) {
        auto malformed = Malformed(MayFail_<ProgramSentence>{}, ERR(121));
        SET_MALFORMED_TOKEN_FIELDS(malformed);
        return malformed;
    }

    std::vector<MayFail<ProgramWord_>> programWords;
    MayFail<ProgramWord_> currentWord;

    LOOP until (input.peek() == EOF || peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
    if (!__first_it)
    {
        if (!consumeSequence(ProgramSentence::CONTINUATOR_SEQUENCE, input)) {
            // this happens when we have an Atom right after a non-Atom (without a space in between)
            auto malformed = Malformed(MayFail_<ProgramSentence>{programWords}, ERR(102));
            SET_MALFORMED_TOKEN_FIELDS(malformed);
            return malformed;
        }
        if (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
            auto malformed = Malformed(MayFail_<ProgramSentence>{programWords}, ERR(122));
            SET_MALFORMED_TOKEN_FIELDS(malformed);
            return malformed;
        }
    }
        currentWord = consumeProgramWord(input);
        programWords.push_back(currentWord);
        if (currentWord.has_error()) {
            auto malformed = Malformed(MayFail_<ProgramSentence>{programWords}, ERR(129));
            SET_MALFORMED_TOKEN_FIELDS(malformed);
            return malformed;
        }

        ENDLOOP
    }

    if (!consumeSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        auto malformed = Malformed(MayFail_<ProgramSentence>{programWords}, ERR(120));
        SET_MALFORMED_TOKEN_FIELDS(malformed);
        return malformed;
    }

    auto sentence = MayFail_<ProgramSentence>{programWords};
    sentence._tokenIndentSpaces = sequenceLen(INDENT_SEQUENCE());
    sentence._tokenLen = GET_INPUT_STREAM_PROGRESS() - sequenceLen(INDENT_SEQUENCE());
    return sentence;
}

///////////////////////////////////////////////////////////

ProgramSentence::ProgramSentence(const std::vector<ProgramWord>& programWords)
        : programWords(programWords){}

ProgramSentence::ProgramSentence(const Term& term) {
    std::vector<ProgramWord> programWords;
    for (auto word: term.words) {
        programWords.push_back(variant_cast(word));
    }
    this->programWords = programWords;
    this->_tokenLen = term._tokenLen;
}

ProgramSentence::operator Term() const {
    std::vector<Word> words;
    for (auto pw: programWords) {
        words.push_back(get_word(pw));
    }
    auto res = Term{words};
    res._tokenLen = this->_tokenLen;
    return res;
}

MayFail_<ProgramSentence>::MayFail_(const std::vector<MayFail<ProgramWord_>>& programWords)
        : programWords(programWords){}

MayFail_<ProgramSentence>::MayFail_(const MayFail_<Term>& term) {
    std::vector<MayFail<ProgramWord_>> programWords;
    for (auto word: term.words) {
        programWords.push_back(mayfail_cast<ProgramWord_>(word));
    }
    this->programWords = programWords;
    this->_tokenLen = term._tokenLen;
}

MayFail_<ProgramSentence>::MayFail_(const ProgramSentence& sentence) {
    std::vector<MayFail<ProgramWord_>> programWords;
    for (auto e: sentence.programWords) {
        programWords.push_back(wrap_pw(e));
    }
    this->programWords = programWords;
    this->_tokenLeadingNewlines = sentence._tokenLeadingNewlines;
    this->_tokenIndentSpaces = sentence._tokenIndentSpaces;
    this->_tokenLen = sentence._tokenLen;
    this->_tokenTrailingNewlines = sentence._tokenTrailingNewlines;
}

MayFail_<ProgramSentence>::operator ProgramSentence() const {
    std::vector<ProgramWord> programWords;
    for (auto e: this->programWords) {
        programWords.push_back(unwrap_pw(e.value()));
    }
    auto sentence = ProgramSentence{programWords};
    sentence._tokenLeadingNewlines = this->_tokenLeadingNewlines;
    sentence._tokenIndentSpaces = this->_tokenIndentSpaces;
    sentence._tokenLen = this->_tokenLen;
    sentence._tokenTrailingNewlines = this->_tokenTrailingNewlines;
    return sentence;
}
