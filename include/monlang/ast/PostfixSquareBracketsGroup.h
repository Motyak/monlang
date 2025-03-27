#ifndef AST_POSTFIX_SQUARE_BRACKETS_GROUP_H
#define AST_POSTFIX_SQUARE_BRACKETS_GROUP_H

#include <monlang/ast/_PostfixLeftPart.h>
#include <monlang/ast/SquareBracketsGroup.h>

struct PostfixSquareBracketsGroup {
    PostfixLeftPart leftPart;
    SquareBracketsGroup rightPart;

    size_t _tokenLen = 0;
    PostfixSquareBracketsGroup() = default;
    PostfixSquareBracketsGroup(const PostfixLeftPart&, const SquareBracketsGroup&);
};

#endif // AST_POSTFIX_SQUARE_BRACKETS_GROUP_H
