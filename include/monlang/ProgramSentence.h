#ifndef PROGRAM_SENTENCE_H
#define PROGRAM_SENTENCE_H

#include <monlang/common.h>
#include <monlang/ProgramWord.h>

#include <vector>
#include <sstream>

struct ProgramSentence {
    static const Sequence SEPARATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<MayFail<ProgramWord>> programWords;
};

MayFail<ProgramSentence> consumeProgramSentence(std::istringstream& input);

#endif // PROGRAM_SENTENCE_H
