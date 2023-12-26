#include <Program.h>
#include <utils/vec-utils.h>

const std::vector<CharacterAppearance> Program::CONTINUATOR_SEQUENCE = { NEWLINE };

const std::vector<char> Program::RESERVED_CHARACTERS = {
    firstChar(CONTINUATOR_SEQUENCE)
};

ProgramSentence consumeProgramSentence(std::istringstream& input) {
    std::vector<char> terminatorCharacters = {firstChar(Program::CONTINUATOR_SEQUENCE)};
    auto term = consumeTerm(input, terminatorCharacters);
    return ProgramSentence{term};
}

Program consumeProgram(std::istringstream& input) {
    std::vector<ProgramSentence> sentences;
    ProgramSentence currentSentence;
    while (input.peek() != EOF) {
        currentSentence = consumeProgramSentence(input);
        consumeSequence(Program::CONTINUATOR_SEQUENCE, input);
        sentences.push_back(currentSentence);
    }
    return Program{sentences};
}