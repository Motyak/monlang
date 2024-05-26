#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <sstream>
#include <optional>
#include <expected>

constexpr char SPACE = 32;
constexpr char NEWLINE = 10;
constexpr char BACKSLASH = 92;

struct Error {
    unsigned code;
    operator unsigned() const;
};

#define OK() return std::expected<void, Error>()

template <typename T>
struct Malformed {
    static_assert(!std::is_void_v<T>);
    T val;
    Error err;
};

template <typename T>
using MayFail = std::expected<T, Malformed<T>>;

template <typename R>
MayFail<R> mayfail_cast(auto inputMayfail) {
    return inputMayfail.transform([](auto t){return R{t};})
            .transform_error([](auto e){return Malformed(R{e.val}, e.err);});
}

struct CharacterAppearance {
    char c;
    unsigned ntimes;
    CharacterAppearance(char c, unsigned ntimes);
    CharacterAppearance(char c);
    operator char() const;
};

using Sequence = std::vector<CharacterAppearance>;

std::optional<char> sequenceFirstChar(const Sequence&);
size_t sequenceLen(const Sequence&);

std::expected<void, Error> consumeSequence(const Sequence&, std::istringstream&);
bool peekSequence(const Sequence&, std::istringstream&);

#endif // COMMON_H
