#ifndef PARENTHESES_GROUP_H
#define PARENTHESES_GROUP_H

#include <monlang/Term.h>
// #include <monlang/PostfixSquareBracketsGroup.h> // moved to .cpp to avoid incomplete type issues ?
// #include <monlang/PostfixParenthesesGroup.h> // moved to .cpp to avoid incomplete type issues ?
#include <monlang/common.h>

#include <sstream>
#include <vector>

struct ParenthesesGroup {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence CONTINUATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<MayFail<Term>> terms;
};

MayFail<ParenthesesGroup> consumeParenthesesGroupStrictly(std::istringstream&);

using consumeParenthesesGroup_RetType = std::variant<
    MayFail<ParenthesesGroup>,
    MayFail<PostfixSquareBracketsGroup>,
    MayFail<PostfixParenthesesGroup>
>;
consumeParenthesesGroup_RetType consumeParenthesesGroup(std::istringstream&);

#endif // PARENTHESES_GROUP_H
