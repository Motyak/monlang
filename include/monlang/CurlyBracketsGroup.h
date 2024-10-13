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
    CurlyBracketsGroup(std::vector<MayFail<ProgramSentence>> sentences);
  protected:
    CurlyBracketsGroup(std::vector<MayFail<ProgramSentence>>, std::optional<MayFail<Term>>);
};
using Subprogram = CurlyBracketsGroup;

struct CurlyBracketsTerm : public CurlyBracketsGroup {
    CurlyBracketsTerm(MayFail<Term> term);
};

MayFail<CurlyBracketsGroup> consumeCurlyBracketsGroup(std::istringstream& input, int indentLevel = 0);

#endif // CURLY_BRACKETS_GROUP_H
