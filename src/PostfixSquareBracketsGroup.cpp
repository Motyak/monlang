#include <monlang/PostfixSquareBracketsGroup.h>
#include <monlang/common.h>

MayFail_<PostfixSquareBracketsGroup> PostfixSquareBracketsGroup::wrap() const {
    return MayFail_<PostfixSquareBracketsGroup>{
        leftPart,
        MayFail_<SquareBracketsGroup>(rightPart)
    };
}

MayFail_<PostfixSquareBracketsGroup>::MayFail_(Word leftPart, MayFail<MayFail_<SquareBracketsGroup>> rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixSquareBracketsGroup>::MayFail_(PostfixSquareBracketsGroup psbg) : MayFail_(psbg.wrap()){}

MayFail_<PostfixSquareBracketsGroup>::operator PostfixSquareBracketsGroup() const {
    return PostfixSquareBracketsGroup{leftPart, (SquareBracketsGroup)rightPart.value()};
}

PostfixSquareBracketsGroup MayFail_<PostfixSquareBracketsGroup>::unwrap() const {
    return (PostfixSquareBracketsGroup)*this;
}
