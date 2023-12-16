#ifndef QUOTATION_H
#define QUOTATION_H

#include <Quoted.h>

#include <vector>
#include <string>
#include <optional>
#include <variant>
#include <sstream>

using QuotedFormat = std::string;

struct Quotation {
    Quoted quoted;
    QuotedFormat quotedFormat = "";

    static const std::vector<char> INITIATOR_SEQUENCE;
    static const std::vector<char> TERMINATOR_SEQUENCE;
    static const std::vector<char> ALT_INITIATOR_SEQUENCE;
    static const std::vector<char> ALT_TERMINATOR_SEQUENCE;

    static const std::vector<std::vector<char>> RESERVED_SEQUENCES;
};

struct Association;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
std::optional<std::variant<Quotation*, Association*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*>> tryConsumeQuotation(std::istringstream&);

#endif // QUOTATION_H
