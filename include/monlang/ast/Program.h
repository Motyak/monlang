#ifndef AST_PROGRAM_H
#define AST_PROGRAM_H

#include <monlang/ast/ProgramSentence.h>

struct Program {
    std::vector<ProgramSentence> sentences;

    size_t _tokenId = 123456789;
    Program() = default;
    Program(const std::vector<ProgramSentence>&);
};

#endif // AST_PROGRAM_H
