#ifndef PROGRAM_WORD_H
#define PROGRAM_WORD_H

#include <monlang/Word.h>
#include <monlang/common.h>

#include <sstream>

struct ProgramWord : public Word {};

MayFail<ProgramWord> consumeProgramWord(std::istringstream&);

#if __has_include (<monlang/mayfail.hpp>)
#ifdef PROGRAM_WORD_CPP
    // enable extern explicit instanciations..
    // ..for common.h 'mayfail' templates
    #include <monlang/mayfail.hpp>
#endif
#endif

#endif // PROGRAM_WORD_H
