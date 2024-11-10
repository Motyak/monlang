#ifndef TERM_H
#define TERM_H

#include <monlang/Word.h>
#include <monlang/common.h>

#include <vector>
#include <sstream>

struct Term {
    static const Sequence CONTINUATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<Word> words;
};

struct MayFail<Term> {
    std::vector<MayFail<Word>> words;
};

MayFail<Term> consumeTerm(const std::vector<char>& terminatorCharacters, std::istringstream& input);

// for SquareBracketsTerm only (very special case)..
MayFail<Term> consumeTerm(const Sequence& terminatorSequence, std::istringstream& input);

// (no terminator at all)
MayFail<Term> consumeTerm(std::istringstream& input);

#endif // TERM_H
