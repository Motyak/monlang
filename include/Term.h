#ifndef TERM_H
#define TERM_H

#include <ProgramWord.h>

#include <vector>
#include <sstream>

struct Term {
    std::vector<ProgramWord> words;

    static const Sequence CONTINUATOR_SEQUENCE;

    static const std::vector<char> RESERVED_CHARACTERS;
};

MayFail<Term> consumeTerm(std::istringstream&, const std::vector<char>& terminatorCharacters);

#endif // TERM_H
