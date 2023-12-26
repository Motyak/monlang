#ifndef PROGRAM_H
#define PROGRAM_H

#include <Term.h>
#include <common.h>

#include <vector>
#include <sstream>

using ProgramSentence = Term;

struct Program {
    std::vector<ProgramSentence> sentences;

    static const std::vector<CharacterAppearance> CONTINUATOR_SEQUENCE;

    static const std::vector<char> RESERVED_CHARACTERS;
};

Program consumeProgram(std::istringstream&);
ProgramSentence consumeProgramSentence(std::istringstream&);

#endif // PROGRAM_H
