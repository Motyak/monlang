#ifndef PARENTHESES_GROUP_H
#define PARENTHESES_GROUP_H

#include <Term.h>

#include <vector>
#include <optional>
#include <variant>
#include <sstream>

struct ParenthesesGroup {
    std::vector<Term> terms;

    static const std::vector<char> INITIATOR_SEQUENCE;
    static const std::vector<char> CONTINUATOR_SEQUENCE;
    static const std::vector<char> TERMINATOR_SEQUENCE;

    static const std::vector<std::vector<char>> RESERVED_SEQUENCES;
};

struct Association;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
std::optional<std::variant<ParenthesesGroup*, Association*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>> tryConsumeParenthesesGroup(std::istringstream&);

#endif // PARENTHESES_GROUP_H
