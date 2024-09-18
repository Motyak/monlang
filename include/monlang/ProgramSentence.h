#ifndef PROGRAM_SENTENCE_H
#define PROGRAM_SENTENCE_H

#include <monlang/common.h>
#include <monlang/ProgramWord.h>

#include <vector>
#include <sstream>

struct ProgramSentence {
    static const Sequence CONTINUATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<MayFail<ProgramWord>> programWords;
};

MayFail<ProgramSentence> consumeProgramSentence(std::istringstream& input, int indentLevel = 0);

#endif // PROGRAM_SENTENCE_H
