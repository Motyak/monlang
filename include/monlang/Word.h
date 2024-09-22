#ifndef WORD_H
#define WORD_H

#include <monlang/common.h>

#include <sstream>
#include <variant>

struct SquareBracketsGroup;
struct ParenthesesGroup;
struct CurlyBracketsGroup;
struct Atom;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;

using Word = std::variant<
    SquareBracketsGroup*, ParenthesesGroup*, CurlyBracketsGroup*, Atom*,
    PostfixParenthesesGroup*, PostfixSquareBracketsGroup*
>;

MayFail<Word> consumeWord(std::istringstream&);

#endif // WORD_H
