#include <ProgramSentence.h>
#include <Program.h>

MayFail<ProgramSentence> consumeProgramSentence(std::istringstream& input) {
    std::vector<char> termChars = {
        sequenceFirstChar(Program::SEPARATOR_SEQUENCE).value()
    };
    return mayfail_cast<ProgramSentence>(consumeTerm(termChars, input));
}
