#ifndef WORD_H
#define WORD_H

#include <monlang/common.h>
#include <monlang/Atom.h>

#include <sstream>
#include <variant>

struct SquareBracketsGroup;
struct ParenthesesGroup;
struct CurlyBracketsGroup;

using Word = std::variant<SquareBracketsGroup*, ParenthesesGroup*, CurlyBracketsGroup*, Atom>;

MayFail<Word> consumeWord(std::istringstream&);

#endif // WORD_H
