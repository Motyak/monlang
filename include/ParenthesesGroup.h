#ifndef PARENTHESES_GROUP_H
#define PARENTHESES_GROUP_H

#include <Term.h>
#include <common.h>

#include <vector>
#include <optional>
#include <variant>
#include <sstream>

struct ParenthesesGroup {
    std::vector<Term> terms;

    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence CONTINUATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;

    static const std::vector<char> RESERVED_CHARACTERS;
};

struct Association;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
std::optional<MayFail<std::variant<ParenthesesGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>>> tryConsumeParenthesesGroup(std::istringstream&);

std::optional<MayFail<ParenthesesGroup*>> tryConsumeParenthesesGroupStrictly(std::istringstream&);

#endif // PARENTHESES_GROUP_H
