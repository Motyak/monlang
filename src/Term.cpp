#include <monlang/Term.h>
#include <monlang/Word.h>
#include <monlang/common.h>

#include <utils/assert-utils.h>
#include <utils/loop-utils.h>

#include <algorithm>

const Sequence Term::CONTINUATOR_SEQUENCE = {SPACE};

const std::vector<char> Term::RESERVED_CHARACTERS = {
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value()
};

static MayFail<MayFail_<Term>> _consumeTerm(const std::vector<Sequence>& terminatorSequences, std::istringstream& input) {
    TRACE_CUR_FUN();
    RECORD_INPUT_STREAM_PROGRESS();

    if (input.peek() == EOF) {
        return Malformed(MayFail_<Term>{}, ERR(135));
    }

    if (peekSequence(Term::CONTINUATOR_SEQUENCE, input)) {
        return Malformed(MayFail_<Term>{}, ERR(131));
    }

    std::vector<MayFail<Word_>> words;
    MayFail<Word_> currentWord;

    LOOP do {
    if (!__first_it)
    {
        if (!consumeSequence(Term::CONTINUATOR_SEQUENCE, input)) {
            // this happens when we have an Atom right after a non-Atom (without a space in between)
            return Malformed(MayFail_<Term>{words}, ERR(103));
        }
    }
        currentWord = consumeWord(input);
        words.push_back(currentWord);
        if (currentWord.has_error()) {
            return Malformed(MayFail_<Term>{words}, ERR(139));
        }

        ENDLOOP
    }
    until (input.peek() == EOF || peekAnySeq(terminatorSequences, input));

    auto term = MayFail_<Term>{words};
    term._tokenLen = GET_INPUT_STREAM_PROGRESS();
    return term;
}

MayFail<MayFail_<Term>> consumeTerm(const std::vector<char>& terminatorCharacters, std::istringstream& input) {
    std::vector<Sequence> terminatorSequences;
    for (auto c: terminatorCharacters) {
        terminatorSequences.push_back({c});
    }
    return _consumeTerm(terminatorSequences, input);
}

MayFail<MayFail_<Term>> consumeTerm(const Sequence& terminatorSequence, std::istringstream& input) {
    return _consumeTerm({terminatorSequence}, input);
}

MayFail<MayFail_<Term>> consumeTerm(std::istringstream& input) {
    return _consumeTerm({}, input);
}

///////////////////////////////////////////////////////////

Term::Term(const std::vector<Word>& words) : words(words){}

MayFail_<Term>::MayFail_(const std::vector<MayFail<Word_>>& words) : words(words){}

MayFail_<Term>::MayFail_(const Term& term) {
    std::vector<MayFail<Word_>> res;
    for (auto e: term.words) {
        res.push_back(wrap_w(e));
    }
    this->words = res;
    this->_tokenLen = term._tokenLen;
}

MayFail_<Term>::operator Term() const {
    std::vector<Word> words;
    for (auto e: this->words) {
        words.push_back(unwrap_w(e.value()));
    }
    auto term = Term{words};
    term._tokenLen = this->_tokenLen;
    return term;
}
