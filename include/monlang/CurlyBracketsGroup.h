#ifndef CURLY_BRACKETS_GROUP_H
#define CURLY_BRACKETS_GROUP_H

#include <monlang/ast/CurlyBracketsGroup.h>

#include <monlang/Program.h>
#include <monlang/Term.h>

template <>
struct MayFail_<CurlyBracketsGroup> : public MayFail_<Program> {
    // inherited from MayFail_<Program> :
    // std::vector<MayFail<MayFail_<ProgramSentence>>> sentences;
    std::optional<MayFail<MayFail_<Term>>> term;

    size_t _tokenLen = 0;
    bool _dollars = false;
    MayFail_() = default;
    explicit MayFail_(const std::vector<MayFail<MayFail_<ProgramSentence>>>&);

    explicit MayFail_(const CurlyBracketsGroup&);
    explicit operator CurlyBracketsGroup() const;

  protected:
    MayFail_(const std::vector<MayFail<MayFail_<ProgramSentence>>>&, const std::optional<MayFail<MayFail_<Term>>>&);
};
using Subprogram_ = MayFail_<CurlyBracketsGroup>;

template <>
struct MayFail_<CurlyBracketsTerm> : public MayFail_<CurlyBracketsGroup> {
    MayFail_(const MayFail<MayFail_<Term>>&);
};

MayFail<MayFail_<CurlyBracketsGroup>> consumeCurlyBracketsGroup(std::istringstream&);

#endif // CURLY_BRACKETS_GROUP_H
