#include <monlang/PostfixSquareBracketsGroup.h>
#include <monlang/common.h>

template <>
PostfixSquareBracketsGroup unwrap(const MayFail_<PostfixSquareBracketsGroup>& psbg) {
    return (PostfixSquareBracketsGroup)psbg;
}

template <>
MayFail_<PostfixSquareBracketsGroup> wrap(const PostfixSquareBracketsGroup& psbg) {
    return MayFail_<PostfixSquareBracketsGroup>{
        psbg.leftPart,
        MayFail_<SquareBracketsGroup>(psbg.rightPart)
    };
}

MayFail_<PostfixSquareBracketsGroup>::MayFail_(Word leftPart, MayFail<MayFail_<SquareBracketsGroup>> rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixSquareBracketsGroup>::MayFail_(PostfixSquareBracketsGroup psbg) : MayFail_(wrap(psbg)){}

MayFail_<PostfixSquareBracketsGroup>::operator PostfixSquareBracketsGroup() const {
    return PostfixSquareBracketsGroup{leftPart, (SquareBracketsGroup)rightPart.value()};
}
