#ifndef CURLY_BRACKETS_GROUP_H
#define CURLY_BRACKETS_GROUP_H

#include <monlang/ProgramSentence.h>
#include <monlang/Term.h>
#include <monlang/common.h>

#include <vector>
#include <optional>

struct CurlyBracketsGroup {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<ProgramSentence> sentences;
    std::optional<Term> term;

    CurlyBracketsGroup() = default;
    CurlyBracketsGroup(std::vector<ProgramSentence>);
  protected:
    CurlyBracketsGroup(std::vector<ProgramSentence>, std::optional<Term>);
};
using Subprogram = CurlyBracketsGroup;

struct CurlyBracketsTerm : public CurlyBracketsGroup {
    CurlyBracketsTerm(Term term);
};

template <>
struct MayFail_<CurlyBracketsGroup> {
    std::vector<MayFail<MayFail_<ProgramSentence>>> sentences;
    std::optional<MayFail<MayFail_<Term>>> term;

    CurlyBracketsGroup unwrap() const {
        return (CurlyBracketsGroup)*this;
    }

    explicit operator CurlyBracketsGroup() const {
        return CurlyBracketsGroup{vec_convert<ProgramSentence>(sentences)};
    }

    MayFail_() = default;
    MayFail_(std::vector<MayFail<MayFail_<ProgramSentence>>>);
  protected:
    MayFail_(std::vector<MayFail<MayFail_<ProgramSentence>>>, std::optional<MayFail<MayFail_<Term>>>);
};
using Subprogram_ = MayFail_<CurlyBracketsGroup>;

template <>
struct MayFail_<CurlyBracketsTerm> : public MayFail_<CurlyBracketsGroup> {
    MayFail_(MayFail<MayFail_<Term>> term);
};

MayFail<MayFail_<CurlyBracketsGroup>> consumeCurlyBracketsGroupStrictly(std::istringstream&);

using consumeCurlyBracketsGroup_RetType = std::variant<
    MayFail<MayFail_<CurlyBracketsGroup>*>,
    MayFail<MayFail_<PostfixParenthesesGroup>*>,
    MayFail<MayFail_<PostfixSquareBracketsGroup>*>
>;

consumeCurlyBracketsGroup_RetType consumeCurlyBracketsGroup(std::istringstream&);

#endif // CURLY_BRACKETS_GROUP_H
