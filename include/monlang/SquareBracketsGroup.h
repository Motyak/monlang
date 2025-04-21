#ifndef SQUARE_BRACKETS_GROUP_H
#define SQUARE_BRACKETS_GROUP_H

#include <monlang/ast/SquareBracketsGroup.h>

#include <monlang/Term.h>

template <>
struct MayFail_<SquareBracketsGroup> {
    std::vector<MayFail<MayFail_<Term>>> terms;

    size_t _tokenLen = 0;
    MayFail_() = default;
    explicit MayFail_(const std::vector<MayFail<MayFail_<Term>>>&);

    explicit MayFail_(const SquareBracketsGroup&);
    explicit operator SquareBracketsGroup() const;
};

MayFail<MayFail_<SquareBracketsGroup>> consumeSquareBracketsGroup(std::istringstream&);

#endif // SQUARE_BRACKETS_GROUP_H
