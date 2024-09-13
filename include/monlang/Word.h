#ifndef WORD_H
#define WORD_H

#include <monlang/common.h>
#include <monlang/Atom.h>

#include <sstream>
#include <variant>

struct SquareBracketsGroup;
struct ParenthesesGroup;

using Word = std::variant<SquareBracketsGroup*, ParenthesesGroup*, Atom>;

MayFail<Word> consumeWord(std::istringstream&);

#endif // WORD_H
