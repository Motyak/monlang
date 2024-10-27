#ifndef POSTFIX_PARENTHESES_GROUP_H
#define POSTFIX_PARENTHESES_GROUP_H

#include <monlang/Word.h>
#include <monlang/ParenthesesGroup.h>

// this "entity" would never be returned if left part was Malformed
struct PostfixParenthesesGroup {
    Word leftPart; // never Malformed, by design
    MayFail<ParenthesesGroup> rightPart;
};

#if __has_include (<monlang/mayfail.hpp>)
#ifdef POSTFIX_PARENTHESES_GROUP_CPP
    // enable extern explicit instanciations..
    // ..for common.h 'mayfail' templates
    #include <monlang/mayfail.hpp>
#endif
#endif

#endif // POSTFIX_PARENTHESES_GROUP_H
