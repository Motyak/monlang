#ifndef AST_PATH_H
#define AST_PATH_H

#include <monlang/ast/_PostfixLeftPart.h>
#include <monlang/ast/Word.h>
#include <monlang/ast/Atom.h>
#include <monlang/common.h>

using PathLeftPart = PostfixLeftPart;

struct Path {
    static const Sequence SEPARATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    PathLeftPart leftPart;
    Atom rightPart;

    size_t _tokenLen = 0;
    Path() = default;
    Path(const PathLeftPart&, const Atom&);
};

#endif // AST_PATH_H
