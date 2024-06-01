#include <monlang/ProgramSentence.h>
#include <monlang/common.h>
#include <monlang/ProgramWord.h>

#include <monlang/Program.h> // in impl only

#include <algorithm>

#define until(x) while(!(x))

const Sequence ProgramSentence::SEPARATOR_SEQUENCE = { SPACE };

const std::vector<char> ProgramSentence::RESERVED_CHARACTERS = {
    sequenceFirstChar(SEPARATOR_SEQUENCE).value()
};

MayFail<ProgramSentence> consumeProgramSentence(std::istringstream& input) {
    std::vector<char> terminatorCharacters = {
        sequenceFirstChar(Program::SEPARATOR_SEQUENCE).value()
    };

    if (input.peek() == EOF) {
        return std::unexpected(Malformed(ProgramSentence{}, Error{117}));
    }

    std::vector<MayFail<ProgramWord>> words;

    words.push_back(consumeProgramWord(input));

    until (input.peek() == EOF || std::any_of(
            terminatorCharacters.begin(),
            terminatorCharacters.end(),
            [&input](auto terminatorChar){return input.peek() == terminatorChar;})) {
        if (!consumeSequence(ProgramSentence::SEPARATOR_SEQUENCE, input)) {
            return std::unexpected(Malformed(ProgramSentence{words}, Error{116}));
        }
        words.push_back(consumeProgramWord(input));
    }

    return ProgramSentence{words};
}
