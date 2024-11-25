#ifndef AST_CURLY_BRACKETS_GROUP_H
#define AST_CURLY_BRACKETS_GROUP_H

#include <monlang/ast/Program.h>
#include <monlang/ast/Term.h>

#include <optional>

struct CurlyBracketsGroup : public Program {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    // inherited from Program
    // std::vector<ProgramSentence> sentences;
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
