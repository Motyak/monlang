#ifndef TERM_H
#define TERM_H

#include <Word.h>

#include <vector>
#include <sstream>

struct Term {
    static const Sequence SEPARATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<Word> words;
};

Term consumeTerm(std::istringstream&);

#endif // TERM_H
