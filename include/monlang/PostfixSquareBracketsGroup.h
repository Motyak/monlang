#ifndef POSTFIX_SQUARE_BRACKETS_GROUP_H
#define POSTFIX_SQUARE_BRACKETS_GROUP_H

#include <monlang/Word.h>
#include <monlang/SquareBracketsGroup.h>

#include <utils/mem-utils.h>
#include <utils/variant-utils.h>

struct PostfixSquareBracketsGroup {
    Word leftPart;
    SquareBracketsGroup rightPart;
};

// this "entity" would never be returned if left part was Malformed
template <>
struct MayFail_<PostfixSquareBracketsGroup> {
    Word_ leftPart; // never Malformed, by design
    MayFail<MayFail_<SquareBracketsGroup>> rightPart;

    PostfixSquareBracketsGroup unwrap() const {
        return (PostfixSquareBracketsGroup)*this;
    }

    explicit operator PostfixSquareBracketsGroup() const {
        return PostfixSquareBracketsGroup{unwrap_w(leftPart), (SquareBracketsGroup)rightPart.val};
    }
};

template <typename T>
MayFail<MayFail_<PostfixSquareBracketsGroup>*>
consumePostfixSquareBracketsGroup(T* accumulatedPostfixLeftPart, std::istringstream& input) {
    auto whats_right_behind = consumeSquareBracketsGroupStrictly(input);
    auto curr_psbg = move_to_heap(MayFail_<PostfixSquareBracketsGroup>{
        variant_cast(*accumulatedPostfixLeftPart),
        whats_right_behind
    });
    if (whats_right_behind.has_error()) {
        return Malformed(curr_psbg, ERR(329));
    }
    *accumulatedPostfixLeftPart = curr_psbg;
    return std::get<MayFail_<PostfixSquareBracketsGroup>*>(*accumulatedPostfixLeftPart);
}

#endif // POSTFIX_SQUARE_BRACKETS_GROUP_H
