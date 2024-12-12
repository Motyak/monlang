#ifndef POSTFIX_PARENTHESES_GROUP_H
#define POSTFIX_PARENTHESES_GROUP_H

#include <monlang/ast/PostfixParenthesesGroup.h>

#include <monlang/Word.h>
#include <monlang/ParenthesesGroup.h>

#include <utils/mem-utils.h>
#include <utils/variant-utils.h>

// this "entity" would never be returned if left part was Malformed
template <>
struct MayFail_<PostfixParenthesesGroup> {
    Word leftPart; // never Malformed, by design
    MayFail<MayFail_<ParenthesesGroup>> rightPart;

    size_t _tokenLen = 0;
    explicit MayFail_(const Word&, const MayFail<MayFail_<ParenthesesGroup>>&);

    explicit MayFail_(const PostfixParenthesesGroup&);
    explicit operator PostfixParenthesesGroup() const;
};

template <typename T>
MayFail<MayFail_<PostfixParenthesesGroup>*>
consumePostfixParenthesesGroup(T* accumulatedPostfixLeftPart, std::istringstream& input) {
    RECORD_INPUT_STREAM_PROGRESS();
    auto left_word = (Word)variant_cast(*accumulatedPostfixLeftPart);
    auto whats_right_behind = consumeParenthesesGroupStrictly(input);

    if (whats_right_behind.has_error()) {
        auto ppg = MayFail_<PostfixParenthesesGroup>{
            left_word,
            whats_right_behind
        };
        return Malformed(move_to_heap(ppg), ERR(319));
    }

    auto ppg = PostfixParenthesesGroup{
        left_word,
        unwrap(whats_right_behind.value())
    };
    ppg._tokenLen = token_len(*accumulatedPostfixLeftPart) + GET_INPUT_STREAM_PROGRESS();
    *accumulatedPostfixLeftPart = move_to_heap(ppg);
    return move_to_heap(wrap(
        *std::get<PostfixParenthesesGroup*>(*accumulatedPostfixLeftPart)
    ));
}

#endif // POSTFIX_PARENTHESES_GROUP_H
