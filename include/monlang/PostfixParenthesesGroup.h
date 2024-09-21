#ifndef POSTFIX_PARENTHESES_GROUP_H
#define POSTFIX_PARENTHESES_GROUP_H

#include <monlang/Word.h>
#include <monlang/ParenthesesGroup.h>

struct PostfixParenthesesGroup {
    Word leftPart;
    ParenthesesGroup rightPart;
};

#endif // POSTFIX_PARENTHESES_GROUP_H
