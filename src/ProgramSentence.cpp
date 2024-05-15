#include <ProgramSentence.h>

MayFail<ProgramSentence> consumeProgramSentence(const std::vector<char>& terminatorCharacters, std::istringstream& input) {
    return mayfail_cast<ProgramSentence>(consumeTerm(terminatorCharacters, input));
}
