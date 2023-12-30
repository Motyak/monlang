#ifndef PARENTHESES_GROUP_H
#define PARENTHESES_GROUP_H

#include <Term.h>

#include <vector>
#include <optional>
#include <variant>
#include <sstream>

struct ParenthesesGroup {
    std::vector<Term> terms;

    static const std::vector<CharacterAppearance> INITIATOR_SEQUENCE;
    static const std::vector<CharacterAppearance> CONTINUATOR_SEQUENCE;
    static const std::vector<CharacterAppearance> TERMINATOR_SEQUENCE;

    static const std::vector<char> RESERVED_CHARACTERS;
};

struct Association;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
std::optional<MayFail<std::variant<ParenthesesGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>>> tryConsumeParenthesesGroup(std::istringstream&);

std::optional<MayFail<ParenthesesGroup*>> tryConsumeParenthesesGroupStrictly(std::istringstream&);

#endif // PARENTHESES_GROUP_H
