#include <monlang/ProgramWord.h>
#include <monlang/Word.h>
#include <monlang/common.h>

MayFail<ProgramWord> consumeProgramWord(std::istringstream& input, int indentLevel) {
    return mayfail_cast<ProgramWord>(consumeWord(input, indentLevel));
}
