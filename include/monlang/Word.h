#ifndef WORD_H
#define WORD_H

#include <monlang/common.h>

#include <sstream>
#include <variant>

struct Atom;
struct ParenthesesGroup;
struct SquareBracketsGroup;
struct CurlyBracketsGroup;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;

using Word = std::variant<
    Atom*,
    ParenthesesGroup*, SquareBracketsGroup*, CurlyBracketsGroup*,
    PostfixParenthesesGroup*, PostfixSquareBracketsGroup*
>;

MayFail<Word> consumeWord(std::istringstream&);

#endif // WORD_H
