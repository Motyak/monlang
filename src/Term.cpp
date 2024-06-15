#include <monlang/Term.h>
#include <monlang/common.h>
#include <monlang/Word.h>

#include <algorithm>

#define until(x) while(!(x))

const Sequence Term::CONTINUATOR_SEQUENCE = { SPACE };

const std::vector<char> Term::RESERVED_CHARACTERS = {
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value()
};

MayFail<Term> consumeTerm(const std::vector<char>& terminatorCharacters, std::istringstream& input) {
    if (input.peek() == EOF) {
        return std::unexpected(Malformed(Term{}, Error{117}));
    }

    std::vector<MayFail<Word>> words;

    words.push_back(consumeWord(input));

    until (input.peek() == EOF || std::any_of(
            terminatorCharacters.begin(),
            terminatorCharacters.end(),
            [&input](auto terminatorChar){return input.peek() == terminatorChar;})) {
        if (!consumeSequence(Term::CONTINUATOR_SEQUENCE, input)) {
            return std::unexpected(Malformed(Term{words}, Error{116}));
        }
        words.push_back(consumeWord(input));
    }

    return Term{words};
}
