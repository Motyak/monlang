#ifndef WORD_H
#define WORD_H

#include <vector>
#include <sstream>

using Word = std::variant<Atom>;

Word consumeWord(std::istringstream&);

#endif // WORD_H
