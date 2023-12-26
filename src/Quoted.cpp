#include <Quoted.h>
#include <Program.h>
#include <common.h>
#include <utils/vec-utils.h>
#include <utils/str-utils.h>

#include <iostream>
#include <algorithm>

Quoted consumeOnelineQuoted(std::istringstream& input) {
    static const std::vector<std::vector<CharacterAppearance>> TERMINATOR_SEQUENCES = {
        Program::CONTINUATOR_SEQUENCE,
        Quotation::TERMINATOR_SEQUENCE
    };
    static const std::vector<CharacterAppearance> ESCAPED_SEQUENCE = vec_concat({BACKSLASH}, Quotation::TERMINATOR_SEQUENCE);

    if (input.peek() == EOF) {
        std::cerr << "unexpected EOF while about to parse a online quoted value" << std::endl;
        throw std::runtime_error("user exception");
    }

    std::string value = "";
    char currentChar;
    while (input.peek() != EOF && std::all_of(
            TERMINATOR_SEQUENCES.begin(),
            TERMINATOR_SEQUENCES.end(),
            [&input](auto seq){return !peekSequence(seq, input);})) {
        if (peekSequence(ESCAPED_SEQUENCE, input)) {
            input.ignore(1); // consume `\`
        }
        input.get(currentChar);
        value += currentChar;
    }

    return Quoted{value};
}

Quoted consumeMultilineQuoted(std::istringstream& input) {
    static const std::vector<std::vector<CharacterAppearance>> TERMINATOR_SEQUENCES = {
        Quotation::ALT_TERMINATOR_SEQUENCE
    };

    if (input.peek() == EOF) {
        std::cerr << "unexpected EOF while about to parse a multiline quoted value" << std::endl;
        throw std::runtime_error("user exception");
    }

    std::string value = "";
    char currentChar;
    while (input.peek() != EOF && std::all_of(
            TERMINATOR_SEQUENCES.begin(),
            TERMINATOR_SEQUENCES.end(),
            [&input](auto seq){return !peekSequence(seq, input);})) {
        input.get(currentChar);
        value += currentChar;
    }

    return Quoted{value};
}
