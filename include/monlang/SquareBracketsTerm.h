#ifndef SQUARE_BRACKETS_TERM_H
#define SQUARE_BRACKETS_TERM_H

#include <monlang/ast/SquareBracketsTerm.h>

#include <monlang/Term.h>
#include <monlang/common.h>

template <>
struct MayFail_<SquareBracketsTerm> {
    MayFail<MayFail_<Term>> term;

    MayFail_() = default;
    explicit MayFail_(MayFail<MayFail_<Term>>);

    explicit MayFail_(SquareBracketsTerm);
    explicit operator SquareBracketsTerm() const;
};

MayFail<MayFail_<SquareBracketsTerm>> consumeSquareBracketsTerm(std::istringstream&);

template <>
SquareBracketsTerm unwrap(const MayFail_<SquareBracketsTerm>&);

template <>
MayFail_<SquareBracketsTerm> wrap(const SquareBracketsTerm&);

#endif // SQUARE_BRACKETS_TERM_H
