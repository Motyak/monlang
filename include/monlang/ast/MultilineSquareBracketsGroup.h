#ifndef AST_MULTILINE_SQUARE_BRACKETS_GROUP_H
#define AST_MULTILINE_SQUARE_BRACKETS_GROUP_H

#include <monlang/ast/ProgramSentence.h>

struct MultilineSquareBracketsGroup {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<ProgramSentence> sentences;

    size_t _tokenLen = 0;
    MultilineSquareBracketsGroup() = default;
    MultilineSquareBracketsGroup(const std::vector<ProgramSentence>&);
};

#endif // AST_MULTILINE_SQUARE_BRACKETS_GROUP_H
