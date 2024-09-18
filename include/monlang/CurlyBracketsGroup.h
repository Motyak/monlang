#ifndef CURLY_BRACKETS_GROUP_H
#define CURLY_BRACKETS_GROUP_H

#include <monlang/common.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Term.h>

#include <vector>
#include <optional>

struct CurlyBracketsGroup {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    /* either */
    std::optional<MayFail<Term>> term;
    std::vector<MayFail<ProgramSentence>> sentences;

    CurlyBracketsGroup() = default;
    CurlyBracketsGroup(std::vector<MayFail<ProgramSentence>> sentences);
  protected:
    CurlyBracketsGroup(std::optional<MayFail<Term>>, std::vector<MayFail<ProgramSentence>>);
};
using Subprogram = CurlyBracketsGroup;

struct CurlyBracketsTerm : public CurlyBracketsGroup {
    CurlyBracketsTerm(MayFail<Term> term);
};

MayFail<CurlyBracketsGroup> consumeCurlyBracketsGroup(std::istringstream&);

#endif // CURLY_BRACKETS_GROUP_H
