#ifndef AST_PROGRAM_SENTENCE_H
#define AST_PROGRAM_SENTENCE_H

#include <monlang/ast/Word.h>
#include <monlang/ast/common.h>

struct ProgramSentence {
    static const Sequence TAB_SEQUENCE;
    static const Sequence CONTINUATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<ProgramWord> programWords;

    size_t _leadingNewlines = 0;
    size_t _tokenLen = 0;
    size_t _trailingNewlines = 0;
    ProgramSentence() = default;
    ProgramSentence(std::vector<ProgramWord>);
};

#endif // AST_PROGRAM_SENTENCE_H
