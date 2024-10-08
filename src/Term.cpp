#include <monlang/Term.h>
#include <monlang/Word.h>
#include <monlang/common.h>

/* in impl only */
#include <utils/assert-utils.h>

#include <algorithm>

#define until(x) while(!(x))

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

    currentWord = consumeWord(input);
    words.push_back(currentWord);
    if (!currentWord.has_value()) {
        return std::unexpected(Malformed(Term{words}, ERR(139)));
    }

    until (input.peek() == EOF || std::any_of(
            terminatorCharacters.begin(),
            terminatorCharacters.end(),
            [&input](auto terminatorChar){return input.peek() == terminatorChar;})) {
        
        if (!consumeSequence(Term::CONTINUATOR_SEQUENCE, input)) {
            // this happens when we have an Atom right after a non-Atom (without a space in between)
            return std::unexpected(Malformed(Term{words}, ERR(103)));
        }

        currentWord = consumeWord(input);
        words.push_back(currentWord);
        if (!currentWord.has_value()) {
            return std::unexpected(Malformed(Term{words}, ERR(139)));
        }
    }

    return Term{words};
}
