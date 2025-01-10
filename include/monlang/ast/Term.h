#ifndef AST_TERM_H
#define AST_TERM_H

#include <monlang/ast/Word.h>
#include <monlang/ast/common.h>

struct Term {
    static const Sequence CONTINUATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<Word> words;

    size_t _tokenLen = 0;
    Term() = default;
    Term(const std::vector<Word>&);
    explicit Term(const Word&); // syntax-sugar for `as_term(word)`
    explicit operator Word() const; // syntax-sugar for `as_word(term)`
};

#endif // AST_TERM_H
