#ifndef PROGRAM_H
#define PROGRAM_H

#include <monlang/ProgramSentence.h>
#include <monlang/common.h>

#include <vector>
#include <sstream>

struct Program {
    std::vector<MayFail<ProgramSentence>> sentences;
};

// struct Program {
//     std::vector<ProgramSentence> sentences;
// };


// template <>
// struct MayFail<Program> {
//     std::vector<ProgramSentence> sentences;
// };

MayFail<Program> consumeProgram(std::istringstream&);

#endif // PROGRAM_H
