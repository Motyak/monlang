#ifndef PARENTHESES_GROUP_H
#define PARENTHESES_GROUP_H

#include <monlang/ast/ParenthesesGroup.h>

#include <monlang/Term.h>

struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
struct Association;

template<>
struct MayFail_<ParenthesesGroup> {
    std::vector<MayFail<MayFail_<Term>>> terms;

    size_t _tokenLen = 0;
    bool _implicit = false;
    MayFail_() = default;
    explicit MayFail_(const std::vector<MayFail<MayFail_<Term>>>&);

    explicit MayFail_(const ParenthesesGroup&);
    explicit operator ParenthesesGroup() const;
};

MayFail<MayFail_<ParenthesesGroup>> consumeParenthesesGroupStrictly(std::istringstream&);

using consumeParenthesesGroup_RetType = std::variant<
    MayFail<MayFail_<ParenthesesGroup>*>,
    MayFail<MayFail_<PostfixParenthesesGroup>*>,
    MayFail<MayFail_<PostfixSquareBracketsGroup>*>,
    MayFail<MayFail_<Association>*>
>;
consumeParenthesesGroup_RetType consumeParenthesesGroup(std::istringstream&);

#endif // PARENTHESES_GROUP_H
