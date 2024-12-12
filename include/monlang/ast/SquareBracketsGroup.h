#ifndef AST_SQUARE_BRACKETS_GROUP_H
#define AST_SQUARE_BRACKETS_GROUP_H

#include <monlang/ast/Term.h>

struct SquareBracketsGroup {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence CONTINUATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<Term> terms;

    size_t _tokenLen = 0;
    SquareBracketsGroup() = default;
    SquareBracketsGroup(std::vector<Term>);
};

#endif // AST_SQUARE_BRACKETS_GROUP_H
