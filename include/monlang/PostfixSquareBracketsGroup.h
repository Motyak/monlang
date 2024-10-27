#ifndef POSTFIX_SQUARE_BRACKETS_GROUP_H
#define POSTFIX_SQUARE_BRACKETS_GROUP_H

#include <monlang/Word.h>
#include <monlang/SquareBracketsGroup.h>

// this "entity" would never be returned if left part was Malformed
struct PostfixSquareBracketsGroup {
    Word leftPart; // never Malformed, by design
    MayFail<SquareBracketsGroup> rightPart;
};

#if __has_include (<monlang/mayfail.hpp>)
#ifdef POSTFIX_SQUARE_BRACKETS_GROUP_CPP
    // enable extern explicit instanciations..
    // ..for common.h 'mayfail' templates
    #include <monlang/mayfail.hpp>
#endif
#endif

#endif // POSTFIX_SQUARE_BRACKETS_GROUP_H
