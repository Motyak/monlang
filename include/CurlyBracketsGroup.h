#ifndef CURLY_BRACKETS_GROUP_H
#define CURLY_BRACKETS_GROUP_H

#include <ProgramSentence.h>

#include <vector>
#include <optional>
#include <variant>
#include <sstream>

struct CurlyBracketsGroup {
    std::vector<ProgramSentence> sentences;

    static const std::vector<char> INITIATOR_SEQUENCE;
    static const std::vector<char> CONTINUATOR_SEQUENCE;
    static const std::vector<char> TERMINATOR_SEQUENCE;

    static const std::vector<char> RESERVED_CHARACTERS;
};

struct Association;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
std::optional<std::variant<CurlyBracketsGroup*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>> tryConsumeCurlyBracketsGroup(std::istringstream&);

std::optional<CurlyBracketsGroup*> tryConsumeCurlyBracketsGroupStrictly(std::istringstream&);

#endif // CURLY_BRACKETS_GROUP_H
