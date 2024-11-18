#ifndef AST_ASSOCIATION_H
#define AST_ASSOCIATION_H

#include <monlang/ast/Word.h>
#include <monlang/ast/common.h>

#include <vector>

using AssociationLeftPart = std::variant<
    Atom*,
    /* no SquareBracketsTerm* here */
    GROUP_ENTITIES,
    POSTFIXES
    /* no Association* here */
>;

struct Association {
    static const Sequence SEPARATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    AssociationLeftPart leftPart;
    Word rightPart;
};

#endif // AST_ASSOCIATION_H
