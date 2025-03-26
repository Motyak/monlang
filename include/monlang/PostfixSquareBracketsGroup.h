#ifndef POSTFIX_SQUARE_BRACKETS_GROUP_H
#define POSTFIX_SQUARE_BRACKETS_GROUP_H

#include <monlang/ast/PostfixSquareBracketsGroup.h>

#include <monlang/Word.h>
#include <monlang/SquareBracketsGroup.h>

// this "entity" would never be returned if left part was Malformed
template <>
struct MayFail_<PostfixSquareBracketsGroup> {
    PostfixLeftPart leftPart; // never Malformed, by design
    MayFail<MayFail_<SquareBracketsGroup>> rightPart;

    size_t _tokenLen = 0;
    explicit MayFail_(const PostfixLeftPart&, const MayFail<MayFail_<SquareBracketsGroup>>&);

    explicit MayFail_(const PostfixSquareBracketsGroup&);
    explicit operator PostfixSquareBracketsGroup() const;
};

Malformed<Malformed_<PostfixSquareBracketsGroup>>
consumePostfixSquareBracketsGroup(PostfixLeftPart&, std::istringstream&);

#endif // POSTFIX_SQUARE_BRACKETS_GROUP_H
