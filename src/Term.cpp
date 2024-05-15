#include <Term.h>

#include <algorithm>

#define until(x) while(!(x))

const Sequence Term::SEPARATOR_SEQUENCE = { SPACE };

const std::vector<char> Term::RESERVED_CHARACTERS = {
    sequenceFirstChar(SEPARATOR_SEQUENCE).value()
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
        if (!consumeSequence(Term::SEPARATOR_SEQUENCE, input)) {
            return std::unexpected(Malformed(Term{words}, Error{116}));
        }
        words.push_back(consumeWord(input));
    }

    return Term{words};
}
