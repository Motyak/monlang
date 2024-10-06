#include <monlang/ProgramSentence.h>
#include <monlang/ProgramWord.h>
#include <monlang/common.h>

#include <utils/assert-util.h>

#include <algorithm>

#define until(x) while(!(x))

const Sequence ProgramSentence::CONTINUATOR_SEQUENCE = { SPACE };
const Sequence ProgramSentence::TERMINATOR_SEQUENCE = { NEWLINE };

const std::vector<char> ProgramSentence::RESERVED_CHARACTERS = {
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value()
};

MayFail<ProgramSentence> consumeProgramSentence(std::istringstream& input, int indentLevel) {
    TRACE_CUR_FUN();
    std::vector<char> terminatorCharacters = {
        sequenceFirstChar(ProgramSentence::TERMINATOR_SEQUENCE).value()
    };

    if (input.peek() == EOF) {
        return std::unexpected(Malformed(ProgramSentence{}, Error{125}));
    }
    // empty sentences are valid, just discarded by the Program
    if (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        input.ignore(1);
        return ProgramSentence{};
    }
    if (indentLevel > 0 && !consumeSequence({{SPACE, 4 * indentLevel}}, input)) {
        return std::unexpected(Malformed(ProgramSentence{}, Error{126}));
    }
    // empty sentences are valid, just discarded by the Program
    if (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        input.ignore(1);
        return ProgramSentence{};
    }
    if (peekSequence(ProgramSentence::CONTINUATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ProgramSentence{}, Error{121}));
    }

    std::vector<MayFail<ProgramWord>> programWords;
    MayFail<ProgramWord> currentWord;

    currentWord = consumeProgramWord(input);
    programWords.push_back(currentWord);
    if (!currentWord.has_value()) {
        return std::unexpected(Malformed(ProgramSentence{programWords}, Error{129}));
    }

    until (input.peek() == EOF || std::any_of(
            terminatorCharacters.begin(),
            terminatorCharacters.end(),
            [&input](auto terminatorChar){return input.peek() == terminatorChar;})) {
        
        if (!consumeSequence(ProgramSentence::CONTINUATOR_SEQUENCE, input)) {
            // this happens when we have an Atom right after a non-Atom (without a space in between)
            return std::unexpected(Malformed(ProgramSentence{programWords}, Error{102}));
        }
        if (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
            return std::unexpected(Malformed(ProgramSentence{programWords}, Error{122}));
        }

        currentWord = consumeProgramWord(input);
        programWords.push_back(currentWord);
        if (!currentWord.has_value()) {
            return std::unexpected(Malformed(ProgramSentence{programWords}, Error{129}));
        }
    }

    if (!consumeSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ProgramSentence{programWords}, Error{120}));
    }

    // eat and discard trailing newlines as well
    while (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        input.ignore(1);
    }

    return ProgramSentence{programWords};
}
