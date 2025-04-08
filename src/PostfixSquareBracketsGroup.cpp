#include <monlang/PostfixSquareBracketsGroup.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/common.h>

/* require knowing all words for token_len() */
#include <monlang/ast/token_len.h>

#include <utils/mem-utils.h>

Malformed<Malformed_<PostfixSquareBracketsGroup>>
consumePostfixSquareBracketsGroup(PostfixLeftPart& accumulatedPostfixLeftPart, std::istringstream& input) {
    RECORD_INPUT_STREAM_PROGRESS();
    auto whats_right_behind = consumeSquareBracketsGroup(input);

    if (whats_right_behind.has_error()) {
        auto psbg = MayFail_<PostfixSquareBracketsGroup>{
            accumulatedPostfixLeftPart,
            whats_right_behind
        };
        return Malformed(psbg, ERR(329));
    }

    auto psbg = PostfixSquareBracketsGroup{
        accumulatedPostfixLeftPart,
        (SquareBracketsGroup)whats_right_behind
    };
    psbg._tokenLen = token_len(accumulatedPostfixLeftPart) + GET_INPUT_STREAM_PROGRESS();
    accumulatedPostfixLeftPart = move_to_heap(psbg);

    // return value is only used in presence of error,..
    // ..in the absence of error what matters is the assignment of the OUT parameter..
    // ..therefore the return value here is useless

    return wrap(psbg);
}

///////////////////////////////////////////////////////////

PostfixSquareBracketsGroup::PostfixSquareBracketsGroup(const PostfixLeftPart& leftPart, const SquareBracketsGroup& rightPart)
        : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixSquareBracketsGroup>::MayFail_(const PostfixLeftPart& leftPart, const MayFail<MayFail_<SquareBracketsGroup>>& rightPart)
        : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixSquareBracketsGroup>::MayFail_(const PostfixSquareBracketsGroup& psbg) {
    this->leftPart = psbg.leftPart;
    this->rightPart = MayFail_<SquareBracketsGroup>(psbg.rightPart);
    this->_suffix = psbg._suffix;
    this->_tokenLen = psbg._tokenLen;
}

MayFail_<PostfixSquareBracketsGroup>::operator PostfixSquareBracketsGroup() const {
    auto psbg = PostfixSquareBracketsGroup{leftPart, (SquareBracketsGroup)rightPart};
    psbg._suffix = this->_suffix;
    psbg._tokenLen = this->_tokenLen;
    return psbg;
}
