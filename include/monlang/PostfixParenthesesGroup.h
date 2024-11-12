#ifndef POSTFIX_PARENTHESES_GROUP_H
#define POSTFIX_PARENTHESES_GROUP_H

#include <monlang/Word.h>
#include <monlang/ParenthesesGroup.h>

#include <utils/mem-utils.h>
#include <utils/variant-utils.h>

struct PostfixParenthesesGroup {
    Word leftPart;
    ParenthesesGroup rightPart;
};

// this "entity" would never be returned if left part was Malformed
template <>
struct MayFail_<PostfixParenthesesGroup> {
    Word_ leftPart; // never Malformed, by design
    MayFail<MayFail_<ParenthesesGroup>> rightPart;

    PostfixParenthesesGroup unwrap() const {
        return (PostfixParenthesesGroup)*this;
    }

    explicit operator PostfixParenthesesGroup() const {
        return PostfixParenthesesGroup{unwrap_w(leftPart), (ParenthesesGroup)rightPart.val};
    }
};

template <typename T>
MayFail<MayFail_<PostfixParenthesesGroup>*>
consumePostfixParenthesesGroup(T* accumulatedPostfixLeftPart, std::istringstream& input) {
    auto whats_right_behind = consumeParenthesesGroupStrictly(input);
    auto curr_ppg = move_to_heap(MayFail_<PostfixParenthesesGroup>{
        variant_cast(*accumulatedPostfixLeftPart),
        whats_right_behind
    });
    if (whats_right_behind.has_error()) {
        return Malformed(curr_ppg, ERR(319));
    }
    *accumulatedPostfixLeftPart = curr_ppg;
    return std::get<MayFail_<PostfixParenthesesGroup>*>(*accumulatedPostfixLeftPart);
}

#endif // POSTFIX_PARENTHESES_GROUP_H
