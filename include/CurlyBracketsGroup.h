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

    static const std::vector<std::vector<char>> RESERVED_SEQUENCES;
};

struct Association;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
std::optional<std::variant<CurlyBracketsGroup*, Association*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>> tryConsumeCurlyBracketsGroup(std::istringstream&);

#endif // CURLY_BRACKETS_GROUP_H
