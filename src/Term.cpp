#include <Term.h>
#include <common.h>

#include <iostream>
#include <algorithm>

const std::vector<char> Term::CONTINUATOR_SEQUENCE = { SPACE };

const std::vector<char> Term::RESERVED_CHARACTERS = {
    Term::CONTINUATOR_SEQUENCE[0]
};

Term consumeTerm(std::istringstream& input, const std::vector<char>& terminatorCharacters) {
    if (!input) {
        std::cerr << "was expecting at least one word in Term, found none" << std::endl;
        exit(1);
    }
    std::vector<ProgramWord> words;
    auto currentWord = consumeProgramWord(input);
    words.push_back(currentWord);
    while (input && std::all_of(
                terminatorCharacters.begin(),
                terminatorCharacters.end(), 
                [&input](auto terminatorChar){return input.peek() != terminatorChar;})) {
        consumeSequence(Term::CONTINUATOR_SEQUENCE, input);
        currentWord = consumeProgramWord(input);
        words.push_back(currentWord);
    }
    return Term{words};
}
