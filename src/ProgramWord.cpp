#define PROGRAM_WORD_CPP

#include <monlang/ProgramWord.h>
#include <monlang/Word.h>
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

MayFail<ProgramWord> consumeProgramWord(std::istringstream& input) {
    return mayfail_cast<ProgramWord>(consumeWord(input));
}
