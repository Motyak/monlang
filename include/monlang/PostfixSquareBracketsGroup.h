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

    size_t _tokenLen = 0;
    explicit MayFail_(Word, MayFail<MayFail_<SquareBracketsGroup>>);

    explicit MayFail_(PostfixSquareBracketsGroup);
    explicit operator PostfixSquareBracketsGroup() const;
};

template <typename T>
MayFail<MayFail_<PostfixSquareBracketsGroup>*>
consumePostfixSquareBracketsGroup(T* accumulatedPostfixLeftPart, std::istringstream& input) {
    RECORD_INPUT_STREAM_PROGRESS();
    auto left_word = (Word)variant_cast(*accumulatedPostfixLeftPart);
    auto whats_right_behind = consumeSquareBracketsGroupStrictly(input);

    if (whats_right_behind.has_error()) {
        auto psbg = MayFail_<PostfixSquareBracketsGroup>{
            left_word,
            whats_right_behind
        };
        return Malformed(move_to_heap(psbg), ERR(319));
    }

    auto psbg = PostfixSquareBracketsGroup{
        left_word,
        unwrap(whats_right_behind.value())
    };
    psbg._tokenLen = token_len(*accumulatedPostfixLeftPart) + GET_INPUT_STREAM_PROGRESS();
    *accumulatedPostfixLeftPart = move_to_heap(psbg);
    return move_to_heap(wrap(
        *std::get<PostfixSquareBracketsGroup*>(*accumulatedPostfixLeftPart)
    ));
}

#endif // POSTFIX_SQUARE_BRACKETS_GROUP_H
