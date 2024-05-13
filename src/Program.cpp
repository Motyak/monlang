#include <Program.h>

#define NEWLINE char(10)

const Sequence Program::SEPARATOR_SEQUENCE = { NEWLINE };

const std::vector<char> Program::RESERVED_CHARACTERS = {
    firstChar(SEPARATOR_SEQUENCE)
};

Program consumeProgram(std::istringstream& input) {
    std::vector<ProgramSentence> sentences;
    ProgramSentence currentSentence;
    while (input.peek() != EOF) {
        currentSentence = consumeProgramSentence(input);
        consumeSequence(Program::SEPARATOR_SEQUENCE, input);
        sentences.push_back(currentSentence);
    }
    return Program{sentences};
}
