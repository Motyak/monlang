#ifndef COMMON_H
#define COMMON_H

#include <monlang/ast/common.h>

#include <utils/mem-utils.h>
#include <utils/assert-utils.h>

#include <string>
#include <optional>
#include <variant>

constexpr char SPACE = 32;
constexpr char NEWLINE = 10;
constexpr char TAB = 9;
constexpr char BACKSLASH = 92;

////////////////////////////////////////////////////////////////

struct Error {
    int code;
    std::string fmt;
    operator int() const;
};
#define ERR(x) Error{atoi(#x), "ERR-"#x}

std::ostream& operator<<(std::ostream&, Error);

template <typename T>
struct MayFail;

template <>
class MayFail<void> {
  public:
    MayFail(Error err) : err(err){} // convenient inside functions returning MayFail<void>
    MayFail(std::optional<Error> err) : err(err){}

    bool has_error() const {return !!err;}
    Error error() const {return err.value();} // throws if no err

    operator bool() const {return !err;} // convenient when calling functions returning MayFail<void>

    std::optional<Error> err;
};

#define OK() return MayFail<void>(std::nullopt)

template <typename T>
class MayFail : public MayFail<void> {
  public:
    MayFail() : MayFail<void>(std::nullopt){}
    MayFail(T val) : MayFail<void>(std::nullopt), val(val){}
    explicit MayFail(T val, std::optional<Error> err) : MayFail<void>(err), val(val){} // keep it explicit

    T value() const {
        ASSERT (!err);
        return val;
    }
    explicit operator T() const {return val;}

    T val; // accessible for simple unwrapping regardless of err
};

template <typename T>
MayFail<T> Malformed(T val, Error err) {
    return MayFail(val, err);
}

template <typename T>
struct MayFail_;

template <typename T>
class MayFail<MayFail_<T>> : public MayFail<void> {
  public:
    MayFail() : MayFail<void>(std::nullopt){}
    MayFail(MayFail_<T> val) : MayFail<void>(std::nullopt), val(val){}
    explicit MayFail(T val) : MayFail<void>(std::nullopt), val(MayFail_<T>(val)){} // keep it explicit
    explicit MayFail(MayFail_<T> val, std::optional<Error> err) : MayFail<void>(err), val(val){} // keep it explicit

    MayFail_<T> value() const {
        ASSERT (!err);
        return val;
    }
    explicit operator T() const {return (T)val;} // the explicit cast to T needs to be provided..
                                                 // ..by each individual MayFail_<> specializations

    MayFail_<T> val;
};

template <typename T>
T unwrap(const MayFail_<T>& mf_) {
    return (T)mf_;
}

template <typename T>
MayFail_<T> wrap(const T& t) {
    return MayFail_<T>(t);
}

template <typename R, typename T>
MayFail<R> mayfail_cast(MayFail<T> inputMayfail) {
    return MayFail(R(inputMayfail.val), inputMayfail.err);
}

template <typename R, typename... Targs>
MayFail<R> mayfail_cast(const std::variant<Targs...>& inputMayfailVariant) {
    return std::visit(
        [](auto inputMayfail){
            return MayFail(R(inputMayfail.val), inputMayfail.err);
        },
        inputMayfailVariant
    );
}

template <typename R, typename T>
MayFail<R> mayfail_convert(MayFail<T> inputMayfail) {
    return MayFail(R(move_to_heap(inputMayfail.val)), inputMayfail.err);
}

////////////////////////////////////////////////////////////////

std::optional<char> sequenceFirstChar(const Sequence&);
size_t sequenceLen(const Sequence&);

MayFail<void> consumeSequence(const Sequence&, std::istringstream&);
bool peekSequence(const Sequence&, std::istringstream&);
bool peekAnyChar(const std::vector<char>&, std::istringstream&);
bool peekAnySeq(const std::vector<Sequence>&, std::istringstream&);

////////////////////////////////////////////////////////////////

#define rdonly

class _TRACE_CUR_FUNC {
    static thread_local int depth;
    std::string funcName;
    rdonly std::istringstream& input; // tracked variable

  public:
    _TRACE_CUR_FUNC(std::string funcName, std::istringstream& input);
    ~_TRACE_CUR_FUNC();
};

#define TRACE_CUR_FUN() \
    auto __trace_obj = _TRACE_CUR_FUNC(__func__, input)

#define RECORD_INPUT_STREAM_PROGRESS() \
    auto ___initial_input_pos = size_t(input.tellg())

#define GET_INPUT_STREAM_PROGRESS() \
    (size_t(input.tellg()) - ___initial_input_pos)

#endif // COMMON_H
