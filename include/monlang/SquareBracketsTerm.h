#ifndef SQUARE_BRACKETS_TERM_H
#define SQUARE_BRACKETS_TERM_H

#include <monlang/ast/SquareBracketsTerm.h>

#include <monlang/Term.h>
#include <monlang/common.h>

template <>
struct MayFail_<SquareBracketsTerm> {
    MayFail<MayFail_<Term>> term;

    size_t _tokenLen = 0;
    size_t _tokenId = 123456789;
    MayFail_() = default;
    explicit MayFail_(const MayFail<MayFail_<Term>>&);

    explicit MayFail_(const SquareBracketsTerm&);
    explicit operator SquareBracketsTerm() const;
};

MayFail<MayFail_<SquareBracketsTerm>> consumeSquareBracketsTerm(std::istringstream&);

#endif // SQUARE_BRACKETS_TERM_H
