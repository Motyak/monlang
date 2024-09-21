#ifndef WORD_H
#define WORD_H

#include <monlang/common.h>

#include <sstream>
#include <variant>

struct SquareBracketsGroup;
struct ParenthesesGroup;
struct CurlyBracketsGroup;
struct Atom;
struct PostfixSquareBracketsGroup;
struct PostfixParenthesesGroup;

using Word = std::variant<
    SquareBracketsGroup*, ParenthesesGroup*, CurlyBracketsGroup*, Atom*,
    PostfixSquareBracketsGroup*, PostfixParenthesesGroup*
>;

MayFail<Word> consumeWord(std::istringstream&);

#endif // WORD_H
