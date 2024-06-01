#ifndef TERM_H
#define TERM_H

#include <monlang/common.h>
#include <monlang/Word.h>

#include <vector>
#include <sstream>

struct Term {
    static const Sequence SEPARATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<MayFail<Word>> words;
};

MayFail<Term> consumeTerm(const std::vector<char>& terminatorCharacters, std::istringstream& input);

#endif // TERM_H
