#ifndef CURLY_BRACKETS_GROUP_H
#define CURLY_BRACKETS_GROUP_H

#include <monlang/common.h>
#include <monlang/ProgramSentence.h>

#include <vector>

struct CurlyBracketsGroup {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<MayFail<ProgramSentence>> sentences;
};
using Subprogram = CurlyBracketsGroup;

MayFail<CurlyBracketsGroup> consumeCurlyBracketsGroup(int indentLevel, std::istringstream&);

#endif // CURLY_BRACKETS_GROUP_H
