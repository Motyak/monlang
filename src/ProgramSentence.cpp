#include <ProgramSentence.h>

ProgramSentence consumeProgramSentence(std::istringstream& input) {
    return consumeTerm(input);
}
