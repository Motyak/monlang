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

    std::vector<MayFail<ProgramSentence>> sentences;
    std::optional<MayFail<Term>> term;

    CurlyBracketsGroup() = default;
    CurlyBracketsGroup(std::vector<ProgramSentence>);
  protected:
    CurlyBracketsGroup(std::vector<ProgramSentence>, std::optional<Term>);
};
using Subprogram = CurlyBracketsGroup;

template <>
struct MayFail<CurlyBracketsGroup> {
    std::vector<ProgramSentence> sentences;
    std::optional<Term> term;

    MayFail<CurlyBracketsGroup>() = default;
    MayFail<CurlyBracketsGroup>(std::vector<MayFail<ProgramSentence>>);
  protected:
    MayFail<CurlyBracketsGroup>(std::vector<MayFail<ProgramSentence>>, std::optional<MayFail<Term>>);
};

struct CurlyBracketsTerm : public CurlyBracketsGroup {
    CurlyBracketsTerm(Term term);
};

template <>
struct MayFail<CurlyBracketsTerm> : public MayFail<CurlyBracketsGroup> {
    MayFail<CurlyBracketsTerm>(MayFail<Term> term);
};

MayFail<CurlyBracketsGroup> consumeCurlyBracketsGroupStrictly(std::istringstream&);

using consumeCurlyBracketsGroup_RetType = std::variant<
    MayFail<CurlyBracketsGroup*>,
    MayFail<PostfixParenthesesGroup*>,
    MayFail<PostfixSquareBracketsGroup*>
>;

consumeCurlyBracketsGroup_RetType consumeCurlyBracketsGroup(std::istringstream&);

#endif // CURLY_BRACKETS_GROUP_H
