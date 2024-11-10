#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/common.h>

MayFail<Program> consumeProgram(std::istringstream& input) {
    TRACE_CUR_FUN();

    std::vector<MayFail<ProgramSentence>> sentences;
    MayFail<ProgramSentence> currentSentence;

    while (input.peek() != EOF) {
        currentSentence = consumeProgramSentence(input);

        if (!currentSentence.has_error() && currentSentence.value().programWords.size() == 0) {
            continue; // ignore empty sentences
        }

        sentences.push_back(currentSentence);

        if (currentSentence.has_error()) {
            return Malformed(Program{sentences}, ERR(119));
        }
    }

    return Program{sentences};
}
