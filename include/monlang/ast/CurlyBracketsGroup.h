#ifndef AST_CURLY_BRACKETS_GROUP_H
#define AST_CURLY_BRACKETS_GROUP_H

#include <monlang/ast/ProgramSentence.h>
#include <monlang/ast/Term.h>

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

#endif // AST_CURLY_BRACKETS_GROUP_H
