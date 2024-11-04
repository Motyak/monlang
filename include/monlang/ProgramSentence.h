#ifndef PROGRAM_SENTENCE_H
#define PROGRAM_SENTENCE_H

#include <monlang/Word.h>
#include <monlang/common.h>

#include <vector>
#include <sstream>

struct ProgramSentence {
    static const Sequence TAB_SEQUENCE;
    static const Sequence CONTINUATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<MayFail<ProgramWord>> programWords;
};

MayFail<ProgramSentence> consumeProgramSentence(std::istringstream& input, int indentLevel = 0);

#define INDENT_SEQUENCE() (indentLevel * ProgramSentence::TAB_SEQUENCE)

#endif // PROGRAM_SENTENCE_H
