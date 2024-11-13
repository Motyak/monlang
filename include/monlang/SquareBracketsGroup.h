#ifndef SQUARE_BRACKETS_GROUP_H
#define SQUARE_BRACKETS_GROUP_H

#include <monlang/Term.h>
#include <monlang/common.h>

#include <utils/vec-utils.h>

#include <sstream>
#include <vector>

struct PostfixSquareBracketsGroup;

struct SquareBracketsGroup {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence CONTINUATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<Term> terms;

    MayFail_<SquareBracketsGroup> wrap() const;
};

template<>
struct MayFail_<SquareBracketsGroup> {
    std::vector<MayFail<MayFail_<Term>>> terms;

    MayFail_() = default;
    MayFail_(std::vector<MayFail<MayFail_<Term>>>);

    explicit MayFail_(SquareBracketsGroup);
    explicit operator SquareBracketsGroup() const;
    SquareBracketsGroup unwrap() const;
};

MayFail<MayFail_<SquareBracketsGroup>> consumeSquareBracketsGroupStrictly(std::istringstream&);

using consumeSquareBracketsGroup_RetType = std::variant<
    MayFail<MayFail_<SquareBracketsGroup>*>,
    MayFail<MayFail_<PostfixSquareBracketsGroup>*>
>;
consumeSquareBracketsGroup_RetType consumeSquareBracketsGroup(std::istringstream&);

#endif // SQUARE_BRACKETS_GROUP_H
