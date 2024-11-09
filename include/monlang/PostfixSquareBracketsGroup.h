#ifndef POSTFIX_SQUARE_BRACKETS_GROUP_H
#define POSTFIX_SQUARE_BRACKETS_GROUP_H

#include <monlang/Word.h>
#include <monlang/SquareBracketsGroup.h>

#include <utils/mem-utils.h>
#include <utils/variant-utils.h>

// this "entity" would never be returned if left part was Malformed
struct PostfixSquareBracketsGroup {
    Word leftPart; // never Malformed, by design
    MayFail<SquareBracketsGroup> rightPart;
};

template <typename T>
MayFail<PostfixSquareBracketsGroup*>
consumePostfixSquareBracketsGroup(T* accumulatedPostfixLeftPart, std::istringstream& input) {
    auto whats_right_behind = consumeSquareBracketsGroupStrictly(input);
    auto curr_psbg = move_to_heap(PostfixSquareBracketsGroup{
        variant_cast(*accumulatedPostfixLeftPart),
        whats_right_behind
    });
    if (!whats_right_behind.has_value()) {
        return std::unexpected(Malformed(curr_psbg, ERR(329)));
    }
    *accumulatedPostfixLeftPart = curr_psbg;
    return std::get<PostfixSquareBracketsGroup*>(*accumulatedPostfixLeftPart);
}

#endif // POSTFIX_SQUARE_BRACKETS_GROUP_H
