#ifndef POSTFIX_PARENTHESES_GROUP_H
#define POSTFIX_PARENTHESES_GROUP_H

#include <monlang/Word.h>
#include <monlang/ParenthesesGroup.h>

// this "entity" would never be returned if left part was Malformed
struct PostfixParenthesesGroup {
    Word leftPart; // never Malformed, by design
    MayFail<ParenthesesGroup> rightPart;
};

#endif // POSTFIX_PARENTHESES_GROUP_H
