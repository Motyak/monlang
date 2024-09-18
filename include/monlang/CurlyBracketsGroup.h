#ifndef CURLY_BRACKETS_GROUP_H
#define CURLY_BRACKETS_GROUP_H

#include <monlang/common.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Term.h>

#include <vector>

struct CurlyBracketsGroup {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<MayFail<ProgramSentence>> sentences;
};
using Subprogram = CurlyBracketsGroup;

struct CurlyBracketsTerm : public CurlyBracketsGroup {
    CurlyBracketsTerm(MayFail<Term> term); // will convert term into program sentences
};

MayFail<CurlyBracketsGroup> consumeCurlyBracketsGroup(int indentLevel, std::istringstream&);

#endif // CURLY_BRACKETS_GROUP_H
