#ifndef WORD_H
#define WORD_H

#include <monlang/common.h>
#include <monlang/Atom.h>

#include <sstream>
#include <variant>

struct SquareBracketsGroup;

using Word = std::variant<SquareBracketsGroup*, Atom>;

MayFail<Word> consumeWord(std::istringstream&);

template <typename T>
MayFail<Word> mayfail_cast(MayFail<T> inputMayfail) {
    return inputMayfail.transform([](T t){return Word{new T(std::move(t))};})
            .transform_error([](Malformed<T> e){return Malformed(Word{new T(std::move(e.val))}, e.err);});
}

// template <typename T>
// MayFail<Word> mayfail_cast(MayFail<T> inputMayfail) {
//     return inputMayfail.transform([](T t){return Word{new T(t)};})
//             .transform_error([](Malformed<T> e){return Malformed(Word{new T(e.val)}, e.err);});
// }

#endif // WORD_H
