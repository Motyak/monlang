#ifndef PROGRAM_H
#define PROGRAM_H

#include <monlang/ProgramSentence.h>
#include <monlang/common.h>

#include <vector>
#include <sstream>

struct Program {
    std::vector<ProgramSentence> sentences;
};

template <>
struct MayFail<Program> : public MayFail<void> {
    std::vector<MayFail<ProgramSentence>> sentences;

    // MayFail(Program prog) : sentences(prog.sentences){}
    // MayFail(Program prog, Error err) : sentences(prog.sentences), MayFail<void>(err){}

    MayFail() = default;
    MayFail(Program prog) : MayFail<void>() {
        std::vector<MayFail<ProgramSentence>> sentences;
        for (auto sentence: prog.sentences) {
            sentences.push_back(MayFail<ProgramSentence>(sentence));
        }
        this->sentences = sentences;
    }
    MayFail(Program prog, Error err) : MayFail<void>(err){
        std::vector<MayFail<ProgramSentence>> sentences;
        for (auto sentence: prog.sentences) {
            sentences.push_back(MayFail<ProgramSentence>(sentence));
        }
        this->sentences = sentences;
    }
};

MayFail<Program> consumeProgram(std::istringstream&);

#endif // PROGRAM_H
