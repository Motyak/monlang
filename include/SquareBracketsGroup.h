#ifndef SQUARE_BRACKETS_GROUP_H
#define SQUARE_BRACKETS_GROUP_H

#include <Term.h>

#include <vector>
#include <optional>
#include <variant>
#include <sstream>

struct SquareBracketsGroup {
    std::vector<Term> terms;

    static const std::vector<char> INITIATOR_SEQUENCE;
    static const std::vector<char> CONTINUATOR_SEQUENCE;
    static const std::vector<char> TERMINATOR_SEQUENCE;

    static const std::vector<std::vector<char>> RESERVED_SEQUENCES;
};

struct Association;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
std::optional<std::variant<SquareBracketsGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>> tryConsumeSquareBracketsGroup(std::istringstream&);

std::optional<SquareBracketsGroup*> tryConsumeSquareBracketsGroupStrictly(std::istringstream&);

#endif // SQUARE_BRACKETS_GROUP_H
