#ifndef POSTFIX_SQUARE_BRACKETS_GROUP_H
#define POSTFIX_SQUARE_BRACKETS_GROUP_H

#include <ProgramWord.h>

struct SquareBracketsGroup;
struct PostfixSquareBracketsGroup {
    ProgramWordWithoutAssociation leftPart;
    SquareBracketsGroup* rightPart;

    // no separator between the two parts, as opposed to Association
};

#endif // POSTFIX_SQUARE_BRACKETS_GROUP_H
