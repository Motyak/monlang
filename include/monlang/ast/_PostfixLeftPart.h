#ifndef AST_POSTFIX_LEFT_PART_H
#define AST_POSTFIX_LEFT_PART_H

#include <monlang/ast/Word.h>

#include <variant>

using PostfixLeftPart = std::variant<
    Atom*,
    Quotation*,
    /* no SquareBracketsTerm* here */
    GROUP_ENTITIES,
    POSTFIXES,
    Path*
    /* no Association* here */
>;

#endif // AST_POSTFIX_LEFT_PART_H
