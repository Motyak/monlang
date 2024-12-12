#include <monlang/PostfixSquareBracketsGroup.h>
#include <monlang/common.h>

PostfixSquareBracketsGroup::PostfixSquareBracketsGroup(Word leftPart, SquareBracketsGroup rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixSquareBracketsGroup>::MayFail_(Word leftPart, MayFail<MayFail_<SquareBracketsGroup>> rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixSquareBracketsGroup>::MayFail_(PostfixSquareBracketsGroup psbg) {
    this->leftPart = psbg.leftPart;
    this->rightPart = MayFail_<SquareBracketsGroup>(psbg.rightPart);
}

MayFail_<PostfixSquareBracketsGroup>::operator PostfixSquareBracketsGroup() const {
    return PostfixSquareBracketsGroup{leftPart, (SquareBracketsGroup)rightPart.value()};
}
