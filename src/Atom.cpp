#include <monlang/Atom.h>
#include <monlang/common.h>

#include <utils/loop-utils.h>
#include <utils/vec-utils.h>

MayFail<Atom> consumeAtom(const std::vector<char>& terminatorCharacters, std::istringstream& input) {
    TRACE_CUR_FUN();
    RECORD_INPUT_STREAM_PROGRESS();

    if (input.peek() == EOF) {
        return Malformed(Atom{}, ERR(995));
    }

    std::string value;
    char currentChar;

    until (input.peek() == EOF || peekAnyChar(terminatorCharacters, input)) {
        input.get(currentChar);
        value += currentChar;
    }

    if (peekSequence({':', ':'}, input)) {
        value += input.get(); // :
        value += input.get(); // :
        while (input.peek() == ':') {
            value += input.get(); // :
        }
        value += consumeAtom(terminatorCharacters, input).val.value;
    }
    if (peekSequence({'.', '.'}, input)) {
        value += input.get(); // .
        value += input.get(); // .
        while (input.peek() == '.') {
            value += input.get(); // .
        }
        value += consumeAtom(terminatorCharacters, input).val.value;
    }

    // means we hit a reserved character
    if (value.size() == 0) {
        return Malformed(Atom{}, ERR(992));
    }

    auto atom = Atom{value};
    atom._tokenLen = GET_INPUT_STREAM_PROGRESS();
    return atom;
}

Atom::Atom(const std::string& value) : value(value){}
