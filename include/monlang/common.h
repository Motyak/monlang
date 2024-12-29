#ifndef COMMON_H
#define COMMON_H

#include <monlang/ast/common.h>

#include <utils/MayFail.h>

constexpr char SPACE = 32;
constexpr char NEWLINE = 10;
constexpr char TAB = 9;
constexpr char BACKSLASH = 92;

////////////////////////////////////////////////////////////////

MayFail<void> consumeSequence(const Sequence&, std::istringstream&);
bool peekSequence(const Sequence&, std::istringstream&);
bool peekStr(const std::string&, std::istringstream&);
bool peekAnyChar(const std::vector<char>&, std::istringstream&);
bool peekAnySeq(const std::vector<Sequence>&, std::istringstream&);

////////////////////////////////////////////////////////////////

#define rdonly

class _TRACE_CUR_FUNC {
    static thread_local int depth;
    std::string funcName;
    rdonly std::istringstream& input; // tracked variable

  public:
    _TRACE_CUR_FUNC(const std::string& funcName, std::istringstream& input);
    ~_TRACE_CUR_FUNC();
};

#define TRACE_CUR_FUN() \
    auto __trace_obj = _TRACE_CUR_FUNC(__func__, input)

////////////////////////////////////////////////////////////////

#define RECORD_INPUT_STREAM_PROGRESS() \
    auto ___initial_input_pos = size_t(input.tellg() == -1? input.str().size() : (size_t)input.tellg())

#define GET_INPUT_STREAM_PROGRESS() \
    (size_t(input.tellg() == -1? input.str().size() : (size_t)input.tellg()) - ___initial_input_pos)

#endif // COMMON_H
