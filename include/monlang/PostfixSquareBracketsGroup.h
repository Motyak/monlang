#ifndef POSTFIX_SQUARE_BRACKETS_GROUP_H
#define POSTFIX_SQUARE_BRACKETS_GROUP_H

#include <monlang/Word.h>
#include <monlang/SquareBracketsGroup.h>

#include <utils/mem-utils.h>
#include <utils/variant-utils.h>

struct PostfixSquareBracketsGroup {
    Word leftPart;
    SquareBracketsGroup rightPart;

    MayFail_<PostfixSquareBracketsGroup> wrap() const;
};

// this "entity" would never be returned if left part was Malformed
template <>
struct MayFail_<PostfixSquareBracketsGroup> {
    Word leftPart; // never Malformed, by design
    MayFail<MayFail_<SquareBracketsGroup>> rightPart;

    MayFail_(Word, MayFail<MayFail_<SquareBracketsGroup>>);

    explicit MayFail_(PostfixSquareBracketsGroup);
    explicit operator PostfixSquareBracketsGroup() const;
    PostfixSquareBracketsGroup unwrap() const;
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
        whats_right_behind.value().unwrap()
    };
    *accumulatedPostfixLeftPart = move_to_heap(ppg);
    return move_to_heap(
        std::get<PostfixSquareBracketsGroup*>(*accumulatedPostfixLeftPart)
            ->wrap()
    );
}

#endif // POSTFIX_SQUARE_BRACKETS_GROUP_H
