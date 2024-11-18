#ifndef SQUARE_BRACKETS_GROUP_H
#define SQUARE_BRACKETS_GROUP_H

#include <monlang/ast/SquareBracketsGroup.h>

#include <monlang/Term.h>

struct PostfixSquareBracketsGroup;

template<>
struct MayFail_<SquareBracketsGroup> {
    std::vector<MayFail<MayFail_<Term>>> terms;

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

template <>
SquareBracketsGroup unwrap(const MayFail_<SquareBracketsGroup>&);

template <>
MayFail_<SquareBracketsGroup> wrap(const SquareBracketsGroup&);

#endif // SQUARE_BRACKETS_GROUP_H
