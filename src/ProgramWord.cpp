#include <monlang/ProgramWord.h>
#include <monlang/common.h>
#include <monlang/Word.h>

MayFail<ProgramWord> consumeProgramWord(std::istringstream& input) {
    return mayfail_cast<ProgramWord>(consumeWord(input));
}
