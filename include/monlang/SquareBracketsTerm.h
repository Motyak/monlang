#ifndef SQUARE_BRACKETS_TERM_H
#define SQUARE_BRACKETS_TERM_H

#include <monlang/Term.h>
#include <monlang/common.h>

struct SquareBracketsTerm {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;

    MayFail<Term> term;
};

MayFail<SquareBracketsTerm> consumeSquareBracketsTerm(std::istringstream&);

#endif // SQUARE_BRACKETS_TERM_H
