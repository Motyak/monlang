#ifndef PROGRAM_H
#define PROGRAM_H

#include <monlang/ProgramSentence.h>
#include <monlang/common.h>

#include <vector>
#include <sstream>

struct Program {
    std::vector<MayFail<ProgramSentence>> sentences;
};

MayFail<Program> consumeProgram(std::istringstream&);

#if __has_include (<monlang/mayfail.hpp>)
#ifdef PROGRAM_CPP
    // enable extern explicit instanciations..
    // ..for common.h 'mayfail' templates
    #include <monlang/mayfail.hpp>
#endif
#endif

#endif // PROGRAM_H
