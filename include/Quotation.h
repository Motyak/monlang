#ifndef QUOTATION_H
#define QUOTATION_H

#include <Quoted.h>
#include <common.h>

#include <vector>
#include <string>
#include <optional>
#include <variant>
#include <sstream>

using QuotedFormat = std::string;

struct Quotation {
    Quoted quoted;
    QuotedFormat quotedFormat = "";

    static const std::vector<CharacterAppearance> INITIATOR_SEQUENCE;
    static const std::vector<CharacterAppearance> TERMINATOR_SEQUENCE;
    static const std::vector<CharacterAppearance> ALT_INITIATOR_SEQUENCE;
    static const std::vector<CharacterAppearance> ALT_TERMINATOR_SEQUENCE;

    static const std::vector<char> RESERVED_CHARACTERS;
};

struct Association;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
std::optional<std::variant<Quotation*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, Association*>> tryConsumeQuotation(std::istringstream&);

std::optional<Quotation*> tryConsumeQuotationStrictly(std::istringstream&);

#endif // QUOTATION_H
