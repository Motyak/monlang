#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <sstream>
#include <optional>
#include <expected>

constexpr char SPACE = 32;
constexpr char NEWLINE = 10;
constexpr char BACKSLASH = 92;

struct ErrorCode {
    unsigned code;
    ErrorCode(unsigned);
    operator unsigned() const;
};

struct Error {
    ErrorCode code;
    // ...
    std::optional<Error*> cause = {};
};

template <typename T>
using MayFail = std::expected<T, Error>;

#define ok() return MayFail<void>()

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

MayFail<void> consumeSequence(const Sequence&, std::istringstream&);
bool peekSequence(const Sequence&, std::istringstream&);

#endif // COMMON_H
