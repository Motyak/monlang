#ifndef COMMON_H
#define COMMON_H

#include <utils/mem-utils.h>
#include <utils/assert-util.h>

#include <vector>
#include <sstream>
#include <optional>
#include <expected>
#include <string>

constexpr char SPACE = 32;
constexpr char NEWLINE = 10;
constexpr char TAB = 9;
constexpr char BACKSLASH = 92;

struct Error {
    int code;
    operator int() const;
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

template <typename R, typename T>
MayFail<R> mayfail_cast(MayFail<T> inputMayfail) {
    return inputMayfail.transform([](auto t){return R{t};})
            .transform_error([](auto e){return Malformed(R{e.val}, e.err);});
}

template <typename R, typename... Targs>
MayFail<R> mayfail_cast(const std::variant<Targs...>& inputMayfailVariant) {
    return std::visit(
        [](auto inputMayfail){
            return inputMayfail.transform([](auto t){return R{t};})
                    .transform_error([](auto e){return Malformed(R{e.val}, e.err);});
        },
        inputMayfailVariant
    );
}

template <typename R, typename T>
MayFail<R> mayfail_convert(MayFail<T> inputMayfail) {
    return inputMayfail.transform([](auto t){return R{move_to_heap(t)};})
            .transform_error([](auto e){return Malformed(R{move_to_heap(e.val)}, e.err);});
}

template <typename R, typename T>
MayFail<R> mayfail_convert(MayFail<T> inputMayfail, auto converter) {
    return inputMayfail.transform([&converter](auto t){return R{converter(t)};})
            .transform_error([&converter](auto e){return Malformed(R{converter(e.val)}, e.err);});
}

template <typename T>
const T& mayfail_unwrap(const MayFail<T>& inputMayfail) {
    return inputMayfail.has_value()?
            inputMayfail.value()
            : inputMayfail.error().val;
}

template <typename T>
T& mayfail_unwrap(MayFail<T>& inputMayfail) {
    return inputMayfail.has_value()?
            inputMayfail.value()
            : inputMayfail.error().val;
}

template <typename T>
std::string serializeErrCode(MayFail<T> malformed) {
    ASSERT(!malformed.has_value());
    auto errCode = malformed.error().err.code;
    return std::string() + (10 <= errCode && errCode <= 99? "0" : "") + std::to_string(errCode);
}

struct CharacterAppearance {
    char c;
    int ntimes;
    CharacterAppearance(char c, int ntimes);
    CharacterAppearance(char c);
    operator char() const; // allow me to cast std::vector<char>..
                           // ..into std::vector<CharacterAppearance> aka Sequence
};

using Sequence = std::vector<CharacterAppearance>;

std::optional<char> sequenceFirstChar(const Sequence&);
size_t sequenceLen(const Sequence&);

std::expected<void, Error> consumeSequence(const Sequence&, std::istringstream&);
bool peekSequence(const Sequence&, std::istringstream&);

#define rdonly

class _TRACE_CUR_FUNC {
    static thread_local int depth;
    std::string funcName;
    rdonly std::istringstream& input; // tracked variable

  public:
    _TRACE_CUR_FUNC(std::string funcName, std::istringstream& input);
    ~_TRACE_CUR_FUNC();
};

#ifdef TRACE
    #include <iostream>
    #define TRACE_CUR_FUN() \
        auto __trace_obj = _TRACE_CUR_FUNC(__func__, input)
#else
    #define TRACE_CUR_FUN()
#endif

#endif // COMMON_H
