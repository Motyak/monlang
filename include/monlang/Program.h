#ifndef PROGRAM_H
#define PROGRAM_H

#include <monlang/ProgramSentence.h>
#include <monlang/common.h>

#include <vector>
#include <sstream>

struct Program {
    std::vector<ProgramSentence> sentences;

    MayFail_<Program> wrap() const;
};

template<>
struct MayFail_<Program> {
    std::vector<MayFail<MayFail_<ProgramSentence>>> sentences;

    MayFail_() = default;
    MayFail_(std::vector<MayFail<MayFail_<ProgramSentence>>>);

    explicit MayFail_(Program);
    explicit operator Program() const;
    Program unwrap() const;
};

MayFail<MayFail_<Program>> consumeProgram(std::istringstream&);

#endif // PROGRAM_H
