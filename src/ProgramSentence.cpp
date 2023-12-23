#include <ProgramSentence.h>
#include <common.h>

const std::vector<CharacterAppearance> ProgramSentence::TERMINATOR_SEQUENCE = { NEWLINE };

const std::vector<char> ProgramSentence::RESERVED_CHARACTERS = {
    firstChar(TERMINATOR_SEQUENCE)
};

ProgramSentence consumeProgramSentence(std::istringstream& input) {
    std::vector<char> terminatorCharacters = {
        firstChar(ProgramSentence::TERMINATOR_SEQUENCE)
    };
    auto term = consumeTerm(input, terminatorCharacters);
    consumeSequence({NEWLINE}, input);
    return ProgramSentence{term};
}
