#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/common.h>

MayFail_<PostfixParenthesesGroup> PostfixParenthesesGroup::wrap() const {
    return MayFail_<PostfixParenthesesGroup>{
        leftPart,
        MayFail_<ParenthesesGroup>(rightPart)
    };
}

MayFail_<PostfixParenthesesGroup>::MayFail_(Word leftPart, MayFail<MayFail_<ParenthesesGroup>> rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixParenthesesGroup>::MayFail_(PostfixParenthesesGroup ppg) : MayFail_(ppg.wrap()){}

MayFail_<PostfixParenthesesGroup>::operator PostfixParenthesesGroup() const {
    return PostfixParenthesesGroup{leftPart, (ParenthesesGroup)rightPart.value()};
}

PostfixParenthesesGroup MayFail_<PostfixParenthesesGroup>::unwrap() const {
    return (PostfixParenthesesGroup)*this;
}
