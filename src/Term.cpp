#include <Term.h>
#include <common.h>

#include <iostream>
#include <algorithm>

const std::vector<CharacterAppearance> Term::CONTINUATOR_SEQUENCE = { SPACE };

const std::vector<char> Term::RESERVED_CHARACTERS = {
    firstChar(CONTINUATOR_SEQUENCE)
};

Term consumeTerm(std::istringstream& input, const std::vector<char>& terminatorCharacters) {
    if (input.peek() == EOF) {
        std::cerr << "was expecting at least one word in Term, found none" << std::endl;
        throw std::runtime_error("user exception");
    }
    std::vector<ProgramWord> words;
    auto currentWord = consumeProgramWord(input);
    words.push_back(currentWord);
    while (input.peek() != EOF && std::all_of(
                terminatorCharacters.begin(),
                terminatorCharacters.end(), 
                [&input](auto terminatorChar){return input.peek() != terminatorChar;})) {
        consumeSequence(Term::CONTINUATOR_SEQUENCE, input);
        currentWord = consumeProgramWord(input);
        words.push_back(currentWord);
    }
    return Term{words};
}
