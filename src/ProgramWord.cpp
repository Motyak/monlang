#include <monlang/ProgramWord.h>

MayFail<ProgramWord> consumeProgramWord(std::istringstream& input) {
    return mayfail_cast<ProgramWord>(consumeWord(input));
}
