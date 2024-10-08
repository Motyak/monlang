#include <monlang/Term.h>
#include <monlang/Word.h>
#include <monlang/common.h>

#include <utils/assert-utils.h>
#include <utils/loop-utils.h>

#include <algorithm>

const Sequence Term::CONTINUATOR_SEQUENCE = { SPACE };

const std::vector<char> Term::RESERVED_CHARACTERS = {
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value()
};

MayFail<Term> consumeTerm(const std::vector<char>& terminatorCharacters, std::istringstream& input) {
    TRACE_CUR_FUN();

    if (input.peek() == EOF) {
        return std::unexpected(Malformed(Term{}, ERR(135)));
    }

    if (peekSequence(Term::CONTINUATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(Term{}, ERR(131)));
    }

    std::vector<MayFail<Word>> words;
    MayFail<Word> currentWord;

    LOOP do {
    if (!__first_it)
    {
        if (!consumeSequence(Term::CONTINUATOR_SEQUENCE, input)) {
            // this happens when we have an Atom right after a non-Atom (without a space in between)
            return std::unexpected(Malformed(Term{words}, ERR(103)));
        }
    }
        currentWord = consumeWord(input);
        words.push_back(currentWord);
        if (!currentWord.has_value()) {
            return std::unexpected(Malformed(Term{words}, ERR(139)));
        }

        ENDLOOP
    }
    until (input.peek() == EOF || peekAnyChar(terminatorCharacters, input));

    return Term{words};
}
