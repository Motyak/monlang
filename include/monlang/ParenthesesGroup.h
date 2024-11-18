#ifndef PARENTHESES_GROUP_H
#define PARENTHESES_GROUP_H

#include <monlang/ast/ParenthesesGroup.h>

#include <monlang/Term.h>

struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;

template<>
struct MayFail_<ParenthesesGroup> {
    std::vector<MayFail<MayFail_<Term>>> terms;

    MayFail_() = default;
    explicit MayFail_(std::vector<MayFail<MayFail_<Term>>>);

    explicit MayFail_(ParenthesesGroup);
    explicit operator ParenthesesGroup() const;
};

MayFail<MayFail_<ParenthesesGroup>> consumeParenthesesGroupStrictly(std::istringstream&);

using consumeParenthesesGroup_RetType = std::variant<
    MayFail<MayFail_<ParenthesesGroup>*>,
    MayFail<MayFail_<PostfixParenthesesGroup>*>,
    MayFail<MayFail_<PostfixSquareBracketsGroup>*>
>;
consumeParenthesesGroup_RetType consumeParenthesesGroup(std::istringstream&);

template <>
ParenthesesGroup unwrap(const MayFail_<ParenthesesGroup>&);

template <>
MayFail_<ParenthesesGroup> wrap(const ParenthesesGroup&);

#endif // PARENTHESES_GROUP_H
