#ifndef POSTFIX_PARENTHESES_GROUP_H
#define POSTFIX_PARENTHESES_GROUP_H

#include <ProgramWord.h>

struct ParenthesesGroup;
struct PostfixParenthesesGroup {
    ProgramWordWithoutAssociation leftPart;
    ParenthesesGroup* rightPart;

    // no separator between the two parts, as opposed to Association
};

#endif // POSTFIX_PARENTHESES_GROUP_H
