#ifndef PROGRAM_H
#define PROGRAM_H

#include <monlang/ProgramSentence.h>
#include <monlang/common.h>

#include <vector>
#include <sstream>

struct Program {
    std::vector<ProgramSentence> sentences;
};

template<>
struct MayFail_<Program> {
    std::vector<MayFail<MayFail_<ProgramSentence>>> sentences;

    explicit operator Program() const {
        return Program{vec_convert<ProgramSentence>(sentences)};
    }
};

MayFail<MayFail_<Program>> consumeProgram(std::istringstream&);

#endif // PROGRAM_H
