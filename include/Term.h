#ifndef TERM_H
#define TERM_H

#include <ProgramWord.h>

#include <vector>

struct Term {
    std::vector<ProgramWord> words;

    static const std::vector<char> CONTINUATOR_SEQUENCE;

    static const std::vector<std::vector<char>> RESERVED_SEQUENCES;
};

#endif // TERM_H
