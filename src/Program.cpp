#include <monlang/Program.h>
#include <monlang/common.h>
#include <monlang/ProgramSentence.h>

MayFail<Program> consumeProgram(std::istringstream& input) {
    std::vector<MayFail<ProgramSentence>> sentences;
    MayFail<ProgramSentence> currentSentence;
    while (input.peek() != EOF) {
        currentSentence = consumeProgramSentence(input);
        sentences.push_back(currentSentence);
    }
    return Program{sentences};
}
