#ifndef PROGRAM_H
#define PROGRAM_H

#include <common.h>
#include <ProgramSentence.h>

#include <vector>
#include <sstream>

struct Program {
    static const Sequence SEPARATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<MayFail<ProgramSentence>> sentences;
};

MayFail<Program> consumeProgram(std::istringstream&);

#endif // PROGRAM_H
