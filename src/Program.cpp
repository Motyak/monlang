#include <monlang/Program.h>
#include <monlang/common.h>
#include <monlang/ProgramSentence.h>

const Sequence Program::SEPARATOR_SEQUENCE = { NEWLINE };

const std::vector<char> Program::RESERVED_CHARACTERS = {
    sequenceFirstChar(SEPARATOR_SEQUENCE).value()
};

MayFail<Program> consumeProgram(std::istringstream& input) {
    std::vector<MayFail<ProgramSentence>> sentences;
    MayFail<ProgramSentence> currentSentence;
    while (input.peek() != EOF) {
        currentSentence = consumeProgramSentence(input);
        if (!consumeSequence(Program::SEPARATOR_SEQUENCE, input)) {
            return std::unexpected(Malformed(Program{sentences}, Error{122}));
        }
        sentences.push_back(currentSentence);
    }
    return Program{sentences};
}
