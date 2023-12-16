#ifndef QUOTATION_H
#define QUOTATION_H

#include <Quoted.h>

#include <vector>
#include <string>

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

#endif // QUOTATION_H
