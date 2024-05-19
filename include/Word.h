#ifndef WORD_H
#define WORD_H

#include <Atom.h>

#include <sstream>
#include <variant>

using Word = std::variant<Atom>;

MayFail<Word> consumeWord(std::istringstream&);

#endif // WORD_H
