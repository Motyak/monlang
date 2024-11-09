#ifndef POSTFIX_PARENTHESES_GROUP_H
#define POSTFIX_PARENTHESES_GROUP_H

#include <monlang/Word.h>
#include <monlang/ParenthesesGroup.h>

#include <utils/mem-utils.h>
#include <utils/variant-utils.h>

// this "entity" would never be returned if left part was Malformed
struct PostfixParenthesesGroup {
    Word leftPart; // never Malformed, by design
    MayFail<ParenthesesGroup> rightPart;
};

template <typename T>
MayFail<PostfixParenthesesGroup*>
consumePostfixParenthesesGroup(T* accumulatedPostfixLeftPart, std::istringstream& input) {
    auto whats_right_behind = consumeParenthesesGroupStrictly(input);
    auto curr_ppg = move_to_heap(PostfixParenthesesGroup{
        variant_cast(*accumulatedPostfixLeftPart),
        whats_right_behind
    });
    if (!whats_right_behind.has_value()) {
        return std::unexpected(Malformed(curr_ppg, ERR(319)));
    }
    *accumulatedPostfixLeftPart = curr_ppg;
    return std::get<PostfixParenthesesGroup*>(*accumulatedPostfixLeftPart);
}

#endif // POSTFIX_PARENTHESES_GROUP_H
