#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/common.h>

template <>
PostfixParenthesesGroup unwrap(const MayFail_<PostfixParenthesesGroup>& ppg) {
    return (PostfixParenthesesGroup)ppg;
}

template <>
MayFail_<PostfixParenthesesGroup> wrap(const PostfixParenthesesGroup& ppg) {
    return MayFail_<PostfixParenthesesGroup>{
        ppg.leftPart,
        MayFail_<ParenthesesGroup>(ppg.rightPart)
    };
}

MayFail_<PostfixParenthesesGroup>::MayFail_(Word leftPart, MayFail<MayFail_<ParenthesesGroup>> rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<PostfixParenthesesGroup>::MayFail_(PostfixParenthesesGroup ppg) : MayFail_(wrap(ppg)){}

MayFail_<PostfixParenthesesGroup>::operator PostfixParenthesesGroup() const {
    return PostfixParenthesesGroup{leftPart, (ParenthesesGroup)rightPart.value()};
}

