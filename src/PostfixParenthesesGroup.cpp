#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/ParenthesesGroup.h>
#include <monlang/common.h>

/* require knowing all words for token_len() */
#include <monlang/ast/token_len.h>

#include <utils/mem-utils.h>

Malformed<Malformed_<PostfixParenthesesGroup>>
consumePostfixParenthesesGroup(PostfixLeftPart& accumulatedPostfixLeftPart, std::istringstream& input) {
    RECORD_INPUT_STREAM_PROGRESS();
    auto whats_right_behind = consumeParenthesesGroup(input);

    if (whats_right_behind.has_error()) {
        auto ppg = MayFail_<PostfixParenthesesGroup>{
            accumulatedPostfixLeftPart,
            whats_right_behind
        };
        return Malformed(ppg, ERR(319));
    }

    auto ppg = PostfixParenthesesGroup{
        accumulatedPostfixLeftPart,
        (ParenthesesGroup)whats_right_behind
    };
    ppg._tokenLen = token_len(accumulatedPostfixLeftPart) + GET_INPUT_STREAM_PROGRESS();
    accumulatedPostfixLeftPart = move_to_heap(ppg);

    // return value is only used in presence of error,..
    // ..in the absence of error what matters is the assignment of the OUT parameter..
    // ..therefore the return value here is useless

    return wrap(ppg);
}

///////////////////////////////////////////////////////////

PostfixParenthesesGroup::PostfixParenthesesGroup(const PostfixLeftPart& leftPart, const ParenthesesGroup& rightPart)
        : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixParenthesesGroup>::MayFail_(const PostfixLeftPart& leftPart, const MayFail<MayFail_<ParenthesesGroup>>& rightPart)
        : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixParenthesesGroup>::MayFail_(const PostfixParenthesesGroup& ppg) {
    this->leftPart = ppg.leftPart;
    this->rightPart = MayFail_<ParenthesesGroup>(ppg.rightPart);
    this->_tokenLen = ppg._tokenLen;
    this->_tokenId = ppg._tokenId;
}

MayFail_<PostfixParenthesesGroup>::operator PostfixParenthesesGroup() const {
    auto ppg = PostfixParenthesesGroup{leftPart, (ParenthesesGroup)rightPart};
    ppg._tokenLen = this->_tokenLen;
    ppg._tokenId = this->_tokenId;
    return ppg;
}

