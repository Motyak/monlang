#ifndef AST_TERM_H
#define AST_TERM_H

#include <monlang/ast/Word.h>
#include <monlang/ast/common.h>

struct Term {
    static const Sequence CONTINUATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<Word> words;
};

#endif // AST_TERM_H
