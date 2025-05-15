#ifndef AST_SQUARE_BRACKETS_TERM_H
#define AST_SQUARE_BRACKETS_TERM_H

#include <monlang/ast/Term.h>

struct SquareBracketsTerm {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;

    Term term;

    size_t _tokenLen = 0;
    size_t _tokenId = 123456789;
    SquareBracketsTerm() = default;
    SquareBracketsTerm(const Term&);
};

#endif // AST_SQUARE_BRACKETS_TERM_H
