#include <monlang/PostfixSquareBracketsGroup.h>
#include <monlang/common.h>

PostfixSquareBracketsGroup::PostfixSquareBracketsGroup(const Word& leftPart, const SquareBracketsGroup& rightPart)
        : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixSquareBracketsGroup>::MayFail_(const Word& leftPart, const MayFail<MayFail_<SquareBracketsGroup>>& rightPart)
        : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixSquareBracketsGroup>::MayFail_(const PostfixSquareBracketsGroup& psbg) {
    this->leftPart = psbg.leftPart;
    this->rightPart = MayFail_<SquareBracketsGroup>(psbg.rightPart);
    this->_tokenLen = psbg._tokenLen;
}

MayFail_<PostfixSquareBracketsGroup>::operator PostfixSquareBracketsGroup() const {
    auto psbg = PostfixSquareBracketsGroup{leftPart, (SquareBracketsGroup)rightPart.value()};
    psbg._tokenLen = this->_tokenLen;
    return psbg;
}
