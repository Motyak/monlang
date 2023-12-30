#ifndef SQUARE_BRACKETS_GROUP_H
#define SQUARE_BRACKETS_GROUP_H

#include <Term.h>
#include <common.h>

#include <vector>
#include <optional>
#include <variant>
#include <sstream>

struct SquareBracketsGroup {
    std::vector<Term> terms;

    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence CONTINUATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;

    static const std::vector<char> RESERVED_CHARACTERS;
};

struct Association;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
/**
The incoming expression in the input stream is considered a `SquareBracketsGroup` if it starts with its initiator.
The function fails if it detects a malformed `SquareBracketsGroup`
The function either returns :
    - a strict `SquareBracketsGroup` (with no composed structure behind it)
    - a `PostfixParenthesesGroup` with a `SquareBracketsGroup` as its left part
    - a `PostfixSquareBracketsGroup` with a `SquareBracketsGroup` as its left part
    - an `Association` of a `SquareBracketsGroup` with another `ProgramWord` as its right part
*/
std::optional<MayFail<std::variant<SquareBracketsGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>>> tryConsumeSquareBracketsGroup(std::istringstream&);

/**
The incoming expression in the input stream is considered a `SquareBracketsGroup` if it starts with its initiator.
The function fails if it detects a malformed `SquareBracketsGroup`.
The function returns a strict `SquareBracketsGroup` (with no composed structure behind it)
*/
std::optional<MayFail<SquareBracketsGroup*>> tryConsumeSquareBracketsGroupStrictly(std::istringstream&);

#endif // SQUARE_BRACKETS_GROUP_H
