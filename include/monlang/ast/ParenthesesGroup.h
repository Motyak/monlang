#ifndef AST_PARENTHESES_GROUP_H
#define AST_PARENTHESES_GROUP_H

#include <monlang/ast/Term.h>

struct ParenthesesGroup {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence CONTINUATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<Term> terms;

    size_t _tokenLen = 0;
    size_t _tokenId = 123456789;
    bool _implicit = false;
    ParenthesesGroup() = default;
    ParenthesesGroup(const std::vector<Term>&);
};

#endif // AST_PARENTHESES_GROUP_H
