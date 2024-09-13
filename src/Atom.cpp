#include <monlang/Atom.h>
#include <monlang/common.h>

#include <algorithm>

#define until(x) while(!(x))

MayFail<Atom> consumeAtom(const std::vector<char>& terminatorCharacters, std::istringstream& input) {
    TRACE_CUR_FUN();

    if (input.peek() == EOF) {
        return std::unexpected(Malformed(Atom{}, Error{991}));
    }

    std::string value;
    char currentChar;
    until (input.peek() == EOF || std::any_of(
            terminatorCharacters.begin(),
            terminatorCharacters.end(),
            [&input](auto terminatorChar){return input.peek() == terminatorChar;})) {
        input.get(currentChar);
        value += currentChar;
    }

    if (value.size() == 0) {
        return std::unexpected(Malformed(Atom{}, Error{992}));
    }

    return Atom{value};
}
