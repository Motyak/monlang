#include <ProgramSentence.h>
#include <common.h>

const std::vector<char> ProgramSentence::TERMINATOR_SEQUENCE = { NEWLINE };

const std::vector<char> ProgramSentence::RESERVED_CHARACTERS = {
    ProgramSentence::TERMINATOR_SEQUENCE[0]
};

ProgramSentence consumeProgramSentence(std::istringstream& input) {
    std::vector<char> terminatorCharacters = {
        ProgramSentence::TERMINATOR_SEQUENCE[0]
    };
    auto term = consumeTerm(input, terminatorCharacters);
    consumeSequence({NEWLINE}, input);
    return ProgramSentence{term};
}
