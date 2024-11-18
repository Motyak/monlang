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

    explicit MayFail_(Word, MayFail<MayFail_<ParenthesesGroup>>);

    explicit MayFail_(PostfixParenthesesGroup);
    explicit operator PostfixParenthesesGroup() const;
};

template <typename T>
MayFail<MayFail_<PostfixParenthesesGroup>*>
consumePostfixParenthesesGroup(T* accumulatedPostfixLeftPart, std::istringstream& input) {
    auto left_word = variant_cast(*accumulatedPostfixLeftPart);
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
    *accumulatedPostfixLeftPart = move_to_heap(ppg);
    return move_to_heap(wrap(
        *std::get<PostfixParenthesesGroup*>(*accumulatedPostfixLeftPart)
    ));
}

template <>
PostfixParenthesesGroup unwrap(const MayFail_<PostfixParenthesesGroup>&);

template <>
MayFail_<PostfixParenthesesGroup> wrap(const PostfixParenthesesGroup&);

#endif // POSTFIX_PARENTHESES_GROUP_H
