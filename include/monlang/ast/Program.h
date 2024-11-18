#ifndef AST_PROGRAM_H
#define AST_PROGRAM_H

#include <monlang/ast/ProgramSentence.h>

struct Program {
    std::vector<ProgramSentence> sentences;
};

#endif // AST_PROGRAM_H
