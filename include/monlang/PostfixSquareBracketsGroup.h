#ifndef POSTFIX_SQUARE_BRACKETS_GROUP_H
#define POSTFIX_SQUARE_BRACKETS_GROUP_H

#include <monlang/Word.h>
#include <monlang/SquareBracketsGroup.h>

// this "entity" would never be returned if left part was Malformed
struct PostfixSquareBracketsGroup {
    Word leftPart; // never Malformed, by design
    MayFail<SquareBracketsGroup> rightPart;
};

#endif // POSTFIX_SQUARE_BRACKETS_GROUP_H
