#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/common.h>

PostfixParenthesesGroup::PostfixParenthesesGroup(Word leftPart, ParenthesesGroup rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixParenthesesGroup>::MayFail_(Word leftPart, MayFail<MayFail_<ParenthesesGroup>> rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixParenthesesGroup>::MayFail_(PostfixParenthesesGroup ppg) {
    this->leftPart = ppg.leftPart;
    this->rightPart = MayFail_<ParenthesesGroup>(ppg.rightPart);
}

MayFail_<PostfixParenthesesGroup>::operator PostfixParenthesesGroup() const {
    return PostfixParenthesesGroup{leftPart, (ParenthesesGroup)rightPart.value()};
}

