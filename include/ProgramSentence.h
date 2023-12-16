#ifndef PROGRAM_SENTENCE_H
#define PROGRAM_SENTENCE_H

#include <Term.h>

#include <vector>

struct ProgramSentence {
    Term term;

    static const std::vector<char> TERMINATOR_SEQUENCE;

    static const std::vector<std::vector<char>> RESERVED_SEQUENCES;
};

#endif // PROGRAM_SENTENCE_H
