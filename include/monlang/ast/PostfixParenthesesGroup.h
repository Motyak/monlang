#ifndef AST_POSTFIX_PARENTHESES_GROUP_H
#define AST_POSTFIX_PARENTHESES_GROUP_H

#include <monlang/ast/Word.h>
#include <monlang/ast/ParenthesesGroup.h>

struct PostfixParenthesesGroup {
    Word leftPart;
    ParenthesesGroup rightPart;
};

#endif // AST_POSTFIX_PARENTHESES_GROUP_H
