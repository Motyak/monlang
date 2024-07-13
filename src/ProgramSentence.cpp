#include <monlang/ProgramSentence.h>
#include <monlang/common.h>
#include <monlang/ProgramWord.h>

/* in impl only */
#include <monlang/Program.h>
#include <utils/assert-util.h>

#include <algorithm>

#define until(x) while(!(x))

const Sequence ProgramSentence::CONTINUATOR_SEQUENCE = { SPACE };
const Sequence ProgramSentence::TERMINATOR_SEQUENCE = { NEWLINE };

const std::vector<char> ProgramSentence::RESERVED_CHARACTERS = {
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value()
};

MayFail<ProgramSentence> consumeProgramSentence(std::istringstream& input) {
    std::vector<char> terminatorCharacters = {
        sequenceFirstChar(ProgramSentence::TERMINATOR_SEQUENCE).value()
    };

    std::vector<MayFail<ProgramWord>> programWords;

    if (peekSequence(ProgramSentence::CONTINUATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ProgramSentence{programWords}, Error{121}));
    }

    // ProgramSentence cannot be empty (special "group" grammar entity)
    if (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ProgramSentence{programWords}, Error{124}));
    }

    programWords.push_back(consumeProgramWord(input));

    until (input.peek() == EOF || std::any_of(
            terminatorCharacters.begin(),
            terminatorCharacters.end(),
            [&input](auto terminatorChar){return input.peek() == terminatorChar;})) {
        
        if (!consumeSequence(ProgramSentence::CONTINUATOR_SEQUENCE, input)) {
            SHOULD_NOT_HAPPEN(); // how could this happen, will see
        }

        if (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
            return std::unexpected(Malformed(ProgramSentence{programWords}, Error{122}));
        }

        programWords.push_back(consumeProgramWord(input));
    }

    if (!consumeSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ProgramSentence{programWords}, Error{120}));
    }

    return ProgramSentence{programWords};
}
