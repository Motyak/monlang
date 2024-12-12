#ifndef AST_POSTFIX_SQUARE_BRACKETS_GROUP_H
#define AST_POSTFIX_SQUARE_BRACKETS_GROUP_H

#include <monlang/ast/Word.h>
#include <monlang/ast/SquareBracketsGroup.h>

struct PostfixSquareBracketsGroup {
    Word leftPart;
    SquareBracketsGroup rightPart;

    size_t _tokenLen = 0;
    PostfixSquareBracketsGroup() = default;
    PostfixSquareBracketsGroup(const Word&, const SquareBracketsGroup&);
};

#endif // AST_POSTFIX_SQUARE_BRACKETS_GROUP_H
