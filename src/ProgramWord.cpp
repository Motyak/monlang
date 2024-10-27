#define PROGRAM_WORD_CPP

#include <monlang/ProgramWord.h>
#include <monlang/Word.h>
#include <monlang/common.h>

MayFail<ProgramWord> consumeProgramWord(std::istringstream& input) {
    return mayfail_cast<ProgramWord>(consumeWord(input));
}

#if __has_include (<monlang/mayfail.hpp>)
    #include "mayfail.tpp"
#endif
