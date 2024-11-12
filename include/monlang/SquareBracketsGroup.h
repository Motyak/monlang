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
};

template<>
struct MayFail_<SquareBracketsGroup> {
    std::vector<MayFail<MayFail_<Term>>> terms;

    SquareBracketsGroup unwrap() const {
        return (SquareBracketsGroup)*this;
    }

    explicit operator SquareBracketsGroup() const {
        return SquareBracketsGroup{vec_convert<Term>(terms)};
    }
};

MayFail<MayFail_<SquareBracketsGroup>> consumeSquareBracketsGroupStrictly(std::istringstream&);

using consumeSquareBracketsGroup_RetType = std::variant<
    MayFail<MayFail_<SquareBracketsGroup>*>,
    MayFail<MayFail_<PostfixSquareBracketsGroup>*>
>;
consumeSquareBracketsGroup_RetType consumeSquareBracketsGroup(std::istringstream&);

#endif // SQUARE_BRACKETS_GROUP_H
