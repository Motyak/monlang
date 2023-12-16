#ifndef PROGRAM_H
#define PROGRAM_H

#include <ProgramSentence.h>

#include <vector>
#include <sstream>

struct Program {
    std::vector<ProgramSentence> sentences;
};

Program consumeProgram(std::istringstream&);

#endif // PROGRAM_H
