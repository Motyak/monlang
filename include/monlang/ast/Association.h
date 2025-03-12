#ifndef AST_ASSOCIATION_H
#define AST_ASSOCIATION_H

#include <monlang/ast/Word.h>
#include <monlang/ast/common.h>

#include <vector>

using AssociationLeftPart = std::variant<
    Atom*,
    Quotation*,
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

    size_t _tokenLen = 0;
    Association() = default;
    Association(const AssociationLeftPart&, const Word&);
};

#endif // AST_ASSOCIATION_H
