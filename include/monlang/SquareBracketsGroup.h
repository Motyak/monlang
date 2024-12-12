#ifndef SQUARE_BRACKETS_GROUP_H
#define SQUARE_BRACKETS_GROUP_H

#include <monlang/ast/SquareBracketsGroup.h>

#include <monlang/Term.h>

struct PostfixSquareBracketsGroup;

template<>
struct MayFail_<SquareBracketsGroup> {
    std::vector<MayFail<MayFail_<Term>>> terms;

    size_t _tokenLen = 0;
    MayFail_() = default;
    explicit MayFail_(std::vector<MayFail<MayFail_<Term>>>);

    explicit MayFail_(SquareBracketsGroup);
    explicit operator SquareBracketsGroup() const;
};

MayFail<MayFail_<SquareBracketsGroup>> consumeSquareBracketsGroupStrictly(std::istringstream&);

using consumeSquareBracketsGroup_RetType = std::variant<
    MayFail<MayFail_<SquareBracketsGroup>*>,
    MayFail<MayFail_<PostfixSquareBracketsGroup>*>
>;
consumeSquareBracketsGroup_RetType consumeSquareBracketsGroup(std::istringstream&);

#endif // SQUARE_BRACKETS_GROUP_H
