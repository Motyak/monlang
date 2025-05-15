#ifndef AST_POSTFIX_SQUARE_BRACKETS_GROUP_H
#define AST_POSTFIX_SQUARE_BRACKETS_GROUP_H

#include <monlang/ast/_PostfixLeftPart.h>
#include <monlang/ast/SquareBracketsGroup.h>

struct PostfixSquareBracketsGroup {
    enum Suffix {
        NONE = 0,
        EXCLAMATION_MARK = 33,
        QUESTION_MARK = 63,
    };

    PostfixLeftPart leftPart;
    SquareBracketsGroup rightPart;

    Suffix _suffix = Suffix::NONE;
    size_t _tokenLen = 0;
    size_t _tokenId = 123456789;
    PostfixSquareBracketsGroup() = default;
    PostfixSquareBracketsGroup(const PostfixLeftPart&, const SquareBracketsGroup&);
};

#endif // AST_POSTFIX_SQUARE_BRACKETS_GROUP_H
