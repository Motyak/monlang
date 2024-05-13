#include <Term.h>

#define SPACE char(32)

const Sequence Term::SEPARATOR_SEQUENCE = { SPACE };

const std::vector<char> Term::RESERVED_CHARACTERS = {
    firstChar(SEPARATOR_SEQUENCE)
};

Term consumeTerm(std::istringstream& input) {
    std::vector<Word> words;
    Word currentWord;
    while (input.peek() != EOF) {
        currentWord = consumeWord(input);
        consumeSequence(Term::SEPARATOR_SEQUENCE, input);
        words.push_back(currentWord);
    }
    return Term{words};
}
