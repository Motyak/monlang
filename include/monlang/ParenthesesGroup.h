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

    std::vector<Term> terms;
};

template<>
struct MayFail_<ParenthesesGroup> {
    std::vector<MayFail<MayFail_<Term>>> terms;

    ParenthesesGroup unwrap() const {
        return (ParenthesesGroup)*this;
    }

    explicit operator ParenthesesGroup() const {
        return ParenthesesGroup{vec_convert<Term>(terms)};
    }
};

MayFail<MayFail_<ParenthesesGroup>> consumeParenthesesGroupStrictly(std::istringstream&);

using consumeParenthesesGroup_RetType = std::variant<
    MayFail<MayFail_<ParenthesesGroup>*>,
    MayFail<MayFail_<PostfixParenthesesGroup>*>,
    MayFail<MayFail_<PostfixSquareBracketsGroup>*>
>;
consumeParenthesesGroup_RetType consumeParenthesesGroup(std::istringstream&);

#endif // PARENTHESES_GROUP_H
