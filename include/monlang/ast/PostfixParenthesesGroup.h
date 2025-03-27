#ifndef AST_POSTFIX_PARENTHESES_GROUP_H
#define AST_POSTFIX_PARENTHESES_GROUP_H

#include <monlang/ast/_PostfixLeftPart.h>
#include <monlang/ast/ParenthesesGroup.h>

struct PostfixParenthesesGroup {
    PostfixLeftPart leftPart;
    ParenthesesGroup rightPart;

    size_t _tokenLen = 0;
    PostfixParenthesesGroup() = default;
    PostfixParenthesesGroup(const PostfixLeftPart&, const ParenthesesGroup&);
};

#endif // AST_POSTFIX_PARENTHESES_GROUP_H
