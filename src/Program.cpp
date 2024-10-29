#define PROGRAM_CPP

#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/common.h>

#if __has_include ("sequence.hpp")
    // enable extern explicit instanciations..
    // ..for common.h 'sequence' templates
    #include "sequence.hpp"
    #include "sequence.tpp"
#endif

#if __has_include ("mayfail.hpp")
    // enable extern explicit instanciations..
    // ..for common.h 'mayfail' templates
    #include "mayfail.hpp"
    #include "mayfail.tpp"
#endif

MayFail<Program> consumeProgram(std::istringstream& input) {
    TRACE_CUR_FUN();

    std::vector<MayFail<ProgramSentence>> sentences;
    MayFail<ProgramSentence> currentSentence;

    while (input.peek() != EOF) {
        currentSentence = consumeProgramSentence(input);

        if (currentSentence.has_value() && currentSentence.value().programWords.size() == 0) {
            continue; // ignore empty sentences
        }

        sentences.push_back(currentSentence);

        if (!currentSentence.has_value()) {
            return std::unexpected(Malformed(Program{sentences}, ERR(119)));
        }
    }

    return Program{sentences};
}
