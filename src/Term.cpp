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

    return MayFail_<Term>{words};
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

template <>
Term unwrap(const MayFail_<Term>& term) {
    return (Term)term;
}

template <>
MayFail_<Term> wrap(const Term& term) {
    std::vector<MayFail<Word_>> res;
    for (auto e: term.words) {
        res.push_back(wrap_w(e));
    }
    return MayFail_<Term>{res};
}

MayFail_<Term>::MayFail_(std::vector<MayFail<Word_>> words) : words(words){}

MayFail_<Term>::MayFail_(Term term) : MayFail_(wrap(term)){}

MayFail_<Term>::operator Term() const {
    std::vector<Word> res;
    for (auto e: words) {
        res.push_back(unwrap_w(e.value()));
    }
    return Term{res};
}
