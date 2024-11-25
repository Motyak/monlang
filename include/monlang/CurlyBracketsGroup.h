#ifndef CURLY_BRACKETS_GROUP_H
#define CURLY_BRACKETS_GROUP_H

#include <monlang/ast/CurlyBracketsGroup.h>

#include <monlang/Program.h>
#include <monlang/Term.h>

template <>
struct MayFail_<CurlyBracketsGroup> : public MayFail_<Program> {
    // inherited from MayFail_<Program>
    // std::vector<MayFail<MayFail_<ProgramSentence>>> sentences;
    std::optional<MayFail<MayFail_<Term>>> term;

    MayFail_() = default;
    explicit MayFail_(std::vector<MayFail<MayFail_<ProgramSentence>>>);

    explicit MayFail_(CurlyBracketsGroup);
    explicit operator CurlyBracketsGroup() const;

  protected:
    MayFail_(std::vector<MayFail<MayFail_<ProgramSentence>>>, std::optional<MayFail<MayFail_<Term>>>);
};
using Subprogram_ = MayFail_<CurlyBracketsGroup>;

template <>
struct MayFail_<CurlyBracketsTerm> : public MayFail_<CurlyBracketsGroup> {
    MayFail_(MayFail<MayFail_<Term>> term);
    explicit MayFail_(CurlyBracketsTerm);
};

MayFail<MayFail_<CurlyBracketsGroup>> consumeCurlyBracketsGroupStrictly(std::istringstream&);

using consumeCurlyBracketsGroup_RetType = std::variant<
    MayFail<MayFail_<CurlyBracketsGroup>*>,
    MayFail<MayFail_<PostfixParenthesesGroup>*>,
    MayFail<MayFail_<PostfixSquareBracketsGroup>*>
>;
consumeCurlyBracketsGroup_RetType consumeCurlyBracketsGroup(std::istringstream&);

template <>
CurlyBracketsGroup unwrap(const MayFail_<CurlyBracketsGroup>&);

template <>
MayFail_<CurlyBracketsGroup> wrap(const CurlyBracketsGroup&);

#endif // CURLY_BRACKETS_GROUP_H
