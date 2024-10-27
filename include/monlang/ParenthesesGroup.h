#ifndef PARENTHESES_GROUP_H
#define PARENTHESES_GROUP_H

#include <monlang/Term.h>
#include <monlang/common.h>

#include <sstream>
#include <vector>

struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;

struct ParenthesesGroup {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence CONTINUATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<MayFail<Term>> terms;
};

MayFail<ParenthesesGroup> consumeParenthesesGroupStrictly(std::istringstream&);

using consumeParenthesesGroup_RetType = std::variant<
    MayFail<ParenthesesGroup*>,
    MayFail<PostfixParenthesesGroup*>,
    MayFail<PostfixSquareBracketsGroup*>
>;
consumeParenthesesGroup_RetType consumeParenthesesGroup(std::istringstream&);

#if __has_include (<monlang/mayfail.hpp>)
#ifdef PARENTHESES_GROUP_CPP
    // enable extern explicit instanciations..
    // ..for common.h 'mayfail' templates
    #include <monlang/mayfail.hpp>
#endif
#endif

#endif // PARENTHESES_GROUP_H
