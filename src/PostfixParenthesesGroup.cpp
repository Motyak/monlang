#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/common.h>

PostfixParenthesesGroup::PostfixParenthesesGroup(const Word& leftPart, const ParenthesesGroup& rightPart)
        : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixParenthesesGroup>::MayFail_(const Word& leftPart, const MayFail<MayFail_<ParenthesesGroup>>& rightPart)
        : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixParenthesesGroup>::MayFail_(const PostfixParenthesesGroup& ppg) {
    this->leftPart = ppg.leftPart;
    this->rightPart = MayFail_<ParenthesesGroup>(ppg.rightPart);
    this->_tokenLen = ppg._tokenLen;
}

MayFail_<PostfixParenthesesGroup>::operator PostfixParenthesesGroup() const {
    auto ppg = PostfixParenthesesGroup{leftPart, (ParenthesesGroup)rightPart.value()};
    ppg._tokenLen = this->_tokenLen;
    return ppg;
}

