#ifndef SQUARE_BRACKETS_TERM_H
#define SQUARE_BRACKETS_TERM_H

#include <monlang/Term.h>
#include <monlang/common.h>

struct SquareBracketsTerm {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;

    Term term;

    MayFail_<SquareBracketsTerm> wrap() const;
};

template <>
struct MayFail_<SquareBracketsTerm> {
    MayFail<MayFail_<Term>> term;

    MayFail_() = default;
    MayFail_(MayFail<MayFail_<Term>>);

    explicit MayFail_(SquareBracketsTerm);
    explicit operator SquareBracketsTerm() const;
    SquareBracketsTerm unwrap() const;
};

MayFail<MayFail_<SquareBracketsTerm>> consumeSquareBracketsTerm(std::istringstream&);

#endif // SQUARE_BRACKETS_TERM_H
