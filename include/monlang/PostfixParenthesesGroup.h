#ifndef POSTFIX_PARENTHESES_GROUP_H
#define POSTFIX_PARENTHESES_GROUP_H

#include <monlang/ast/PostfixParenthesesGroup.h>

#include <monlang/Word.h>
#include <monlang/ParenthesesGroup.h>

// this "entity" would never be returned if left part was Malformed
template <>
struct MayFail_<PostfixParenthesesGroup> {
    PostfixLeftPart leftPart; // never Malformed, by design
    MayFail<MayFail_<ParenthesesGroup>> rightPart;

    size_t _tokenLen = 0;
    explicit MayFail_(const PostfixLeftPart&, const MayFail<MayFail_<ParenthesesGroup>>&);

    explicit MayFail_(const PostfixParenthesesGroup&);
    explicit operator PostfixParenthesesGroup() const;
};

Malformed<Malformed_<PostfixParenthesesGroup>*>
consumePostfixParenthesesGroup(PostfixLeftPart&, std::istringstream&);

#endif // POSTFIX_PARENTHESES_GROUP_H
