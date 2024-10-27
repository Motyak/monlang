#include <monlang/ProgramWord.h>
#include <monlang/Word.h>
#include <monlang/common.h>

MayFail<ProgramWord> consumeProgramWord(std::istringstream& input) {
    return mayfail_cast<ProgramWord>(consumeWord(input));
}

#if __has_include (<mayfail.hpp>)
    #define PROGRAM_WORD_CPP
    #include "mayfail.tpp"
#endif
