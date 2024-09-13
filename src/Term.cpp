#include <monlang/Term.h>
#include <monlang/common.h>
#include <monlang/Word.h>

/* in impl only */
#include <utils/assert-util.h>

#include <algorithm>

#define until(x) while(!(x))

const Sequence Term::CONTINUATOR_SEQUENCE = { SPACE };

const std::vector<char> Term::RESERVED_CHARACTERS = {
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value()
};

MayFail<Term> consumeTerm(const std::vector<char>& terminatorCharacters, std::istringstream& input) {
    std::cerr << "DEBUG consumeTerm: `" << input.str().substr(input.tellg()) << "`" << std::endl;
    if (input.peek() == EOF) {
        return std::unexpected(Malformed(Term{}, Error{135}));
    }
    if (peekSequence(Term::CONTINUATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(Term{}, Error{131}));
    }

    std::vector<MayFail<Word>> words;
    MayFail<Word> currentWord;

    currentWord = consumeWord(input);
    words.push_back(currentWord);
    if (!currentWord.has_value()) {
        return std::unexpected(Malformed(Term{words}, Error{139}));
    }

    until (input.peek() == EOF || std::any_of(
            terminatorCharacters.begin(),
            terminatorCharacters.end(),
            [&input](auto terminatorChar){return input.peek() == terminatorChar;})) {
        
        if (!consumeSequence(Term::CONTINUATOR_SEQUENCE, input)) {
            std::cerr << "DEBUG fdsfds: `" << input.str().substr(input.tellg()) << "`" << std::endl;
            SHOULD_NOT_HAPPEN(); // cannot happen because Atom's error drops first
        }

        currentWord = consumeWord(input);
        words.push_back(currentWord);
        if (!currentWord.has_value()) {
            return std::unexpected(Malformed(Term{words}, Error{139}));
        }
    }

    return Term{words};
}
