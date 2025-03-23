#include <monlang/PostfixSquareBracketsGroup.h>
#include <monlang/Word.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/common.h>

/* require knowing all words for token_len() */
#include <monlang/ast/Atom.h>
#include <monlang/ast/Quotation.h>
#include <monlang/ast/ParenthesesGroup.h>
#include <monlang/ast/CurlyBracketsGroup.h>
#include <monlang/ast/PostfixParenthesesGroup.h>
#include <monlang/ast/Path.h>
#include <monlang/ast/Association.h>

#include <utils/mem-utils.h>

Malformed<Malformed_<PostfixSquareBracketsGroup>*>
consumePostfixSquareBracketsGroup(PostfixLeftPart& accumulatedPostfixLeftPart, std::istringstream& input) {
    RECORD_INPUT_STREAM_PROGRESS();
    auto whats_right_behind = consumeSquareBracketsGroup(input);

    if (whats_right_behind.has_error()) {
        auto ppg = MayFail_<PostfixSquareBracketsGroup>{
            accumulatedPostfixLeftPart,
            whats_right_behind
        };
        return Malformed(move_to_heap(ppg), ERR(319));
    }

    auto ppg = PostfixSquareBracketsGroup{
        accumulatedPostfixLeftPart,
        unwrap(whats_right_behind.value())
    };
    ppg._tokenLen = token_len(accumulatedPostfixLeftPart) + GET_INPUT_STREAM_PROGRESS();
    accumulatedPostfixLeftPart = move_to_heap(ppg);

    // return value is only used in presence of error,..
    // ..in the absence of error what matters is the assignment of the OUT parameter..
    // ..therefore the return value here is useless

    // return move_to_heap(wrap(ppg));
    return nullptr;
}

///////////////////////////////////////////////////////////

PostfixSquareBracketsGroup::PostfixSquareBracketsGroup(const PostfixLeftPart& leftPart, const SquareBracketsGroup& rightPart)
        : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixSquareBracketsGroup>::MayFail_(const PostfixLeftPart& leftPart, const MayFail<MayFail_<SquareBracketsGroup>>& rightPart)
        : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixSquareBracketsGroup>::MayFail_(const PostfixSquareBracketsGroup& ppg) {
    this->leftPart = ppg.leftPart;
    this->rightPart = MayFail_<SquareBracketsGroup>(ppg.rightPart);
    this->_tokenLen = ppg._tokenLen;
}

MayFail_<PostfixSquareBracketsGroup>::operator PostfixSquareBracketsGroup() const {
    auto ppg = PostfixSquareBracketsGroup{leftPart, (SquareBracketsGroup)rightPart.value()};
    ppg._tokenLen = this->_tokenLen;
    return ppg;
}
