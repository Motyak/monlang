#ifndef SQUARE_BRACKETS_TERM_H
#define SQUARE_BRACKETS_TERM_H

#include <monlang/Term.h>
#include <monlang/common.h>

struct SquareBracketsTerm {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;

    Term term;
};

template <>
struct MayFail_<SquareBracketsTerm> {
    MayFail<MayFail_<Term>> term;

    SquareBracketsTerm unwrap() const {
        return (SquareBracketsTerm)*this; // avoid duplication
    }

    explicit operator SquareBracketsTerm() const {
        return SquareBracketsTerm{(Term)term.value()};
    }
};

MayFail<MayFail_<SquareBracketsTerm>> consumeSquareBracketsTerm(std::istringstream&);

#endif // SQUARE_BRACKETS_TERM_H
