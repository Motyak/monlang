#include <Quoted.h>
#include <common.h>
#include <utils/vec-utils.h>
#include <utils/str-utils.h>

#include <iostream>
#include <algorithm>

Quoted consumeQuoted(std::istringstream& input, const std::vector<CharacterAppearance>& terminatorSequence) {
    static const std::vector<CharacterAppearance> ESCAPED_SEQUENCE = vec_concat({BACKSLASH}, terminatorSequence);

    if (input.peek() == EOF) {
        std::cerr << "unexpected EOF while about to parse a quoted value" << std::endl;
        throw std::runtime_error("user exception");
    }

    std::string value = "";
    char currentChar;
    while (input.peek() != EOF && !peekSequence(terminatorSequence, input)) {
        if (peekSequence(ESCAPED_SEQUENCE, input)) {
            input.ignore(1); // consume `\`
        }
        input.get(currentChar);
        value += currentChar;
    }

    return Quoted{value};
}
