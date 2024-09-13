#ifndef COMMON_H
#define COMMON_H

#include <utils/mem-utils.h>
#include <utils/assert-util.h>

#include <vector>
#include <sstream>
#include <optional>
#include <expected>
#include <string>

#ifdef TRACE
    #include <iostream>
    #define TRACE_CUR_FUN() \
        std::cerr << "DEBUG " << __func__ << ": `" \
                << input.str().substr(input.tellg()) << "`\n"
#else
    #define TRACE_CUR_FUN()
#endif

constexpr char SPACE = 32;
constexpr char NEWLINE = 10;
constexpr char TAB = 9;
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

template <typename R>
MayFail<R> mayfail_convert(auto inputMayfail) {
    return inputMayfail.transform([](auto t){return R{move_to_heap(t)};})
            .transform_error([](auto e){return Malformed(R{move_to_heap(e.val)}, e.err);});
}

template <typename R>
MayFail<R> mayfail_convert(auto inputMayfail, auto converter) {
    return inputMayfail.transform([&converter](auto t){return R{converter(t)};})
            .transform_error([&converter](auto e){return Malformed(R{converter(e.val)}, e.err);});
}

// template <typename T>
// T mayfail_unwrap(MayFail<T> inputMayfail) {
//     return inputMayfail.has_value()? 
//     		inputMayfail.value()
//     		: inputMayfail.error().val;
// }

template <typename T>
std::string serializeErrCode(MayFail<T> malformed) {
    ASSERT(!malformed.has_value());
    auto errCode = malformed.error().err.code;
    return std::string() + (10 <= errCode && errCode <= 99? "0" : "") + std::to_string(errCode);
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
