#ifndef PROGRAM_H
#define PROGRAM_H

#include <monlang/ast/Program.h>

#include <monlang/ProgramSentence.h>
#include <monlang/common.h>

template<>
struct MayFail_<Program> {
    std::vector<MayFail<MayFail_<ProgramSentence>>> sentences;

    MayFail_() = default;
    explicit MayFail_(std::vector<MayFail<MayFail_<ProgramSentence>>>);

    explicit MayFail_(Program);
    explicit operator Program() const;
};

MayFail<MayFail_<Program>> consumeProgram(std::istringstream&);

#endif // PROGRAM_H
