#ifndef POSTFIX_SQUARE_BRACKETS_GROUP_H
#define POSTFIX_SQUARE_BRACKETS_GROUP_H

#include <monlang/ast/PostfixSquareBracketsGroup.h>

#include <monlang/Word.h>
#include <monlang/SquareBracketsGroup.h>

#include <utils/mem-utils.h>
#include <utils/variant-utils.h>

// this "entity" would never be returned if left part was Malformed
template <>
struct MayFail_<PostfixSquareBracketsGroup> {
    Word leftPart; // never Malformed, by design
    MayFail<MayFail_<SquareBracketsGroup>> rightPart;

    explicit MayFail_(Word, MayFail<MayFail_<SquareBracketsGroup>>);

    explicit MayFail_(PostfixSquareBracketsGroup);
    explicit operator PostfixSquareBracketsGroup() const;
};

template <typename T>
MayFail<MayFail_<PostfixSquareBracketsGroup>*>
consumePostfixSquareBracketsGroup(T* accumulatedPostfixLeftPart, std::istringstream& input) {
    auto left_word = variant_cast(*accumulatedPostfixLeftPart);
    auto whats_right_behind = consumeSquareBracketsGroupStrictly(input);

    if (whats_right_behind.has_error()) {
        auto ppg = MayFail_<PostfixSquareBracketsGroup>{
            left_word,
            whats_right_behind
        };
        return Malformed(move_to_heap(ppg), ERR(319));
    }

    auto ppg = PostfixSquareBracketsGroup{
        left_word,
        unwrap(whats_right_behind.value())
    };
    *accumulatedPostfixLeftPart = move_to_heap(ppg);
    return move_to_heap(wrap(
        *std::get<PostfixSquareBracketsGroup*>(*accumulatedPostfixLeftPart)
    ));
}

template <>
PostfixSquareBracketsGroup unwrap(const MayFail_<PostfixSquareBracketsGroup>&);

template <>
MayFail_<PostfixSquareBracketsGroup> wrap(const PostfixSquareBracketsGroup&);

#endif // POSTFIX_SQUARE_BRACKETS_GROUP_H
