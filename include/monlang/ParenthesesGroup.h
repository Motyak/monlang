#ifndef PARENTHESES_GROUP_H
#define PARENTHESES_GROUP_H

#include <monlang/ast/ParenthesesGroup.h>

#include <monlang/Term.h>

template<>
struct MayFail_<ParenthesesGroup> {
    std::vector<MayFail<MayFail_<Term>>> terms;

    size_t _tokenLen = 0;
    size_t _tokenId = 123456789;
    bool _implicit = false;
    MayFail_() = default;
    explicit MayFail_(const std::vector<MayFail<MayFail_<Term>>>&);

    explicit MayFail_(const ParenthesesGroup&);
    explicit operator ParenthesesGroup() const;
};

MayFail<MayFail_<ParenthesesGroup>> consumeParenthesesGroup(std::istringstream&);

#endif // PARENTHESES_GROUP_H
