#include <monlang/ProgramSentence.h>
#include <monlang/ProgramWord.h>
#include <monlang/common.h>

#include <utils/assert-utils.h>
#include <utils/loop-utils.h>

#include <algorithm>

const Sequence ProgramSentence::CONTINUATOR_SEQUENCE = { SPACE };
const Sequence ProgramSentence::TERMINATOR_SEQUENCE = { NEWLINE };

const std::vector<char> ProgramSentence::RESERVED_CHARACTERS = {
    sequenceFirstChar(CONTINUATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value()
};

MayFail<ProgramSentence> consumeProgramSentence(std::istringstream& input, int indentLevel) {
    TRACE_CUR_FUN();

    if (input.peek() == EOF) {
        return std::unexpected(Malformed(ProgramSentence{}, ERR(125)));
    }

    // empty sentences are valid, just discarded by the Program
    if (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(ProgramSentence::TERMINATOR_SEQUENCE));
        return ProgramSentence{};
    }

    if (indentLevel > 0 && !consumeSequence({{SPACE, 4 * indentLevel}}, input)) {
        return std::unexpected(Malformed(ProgramSentence{}, ERR(123)));
    }

    // indented empty sentences are valid, just discarded by the Program
    if (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(ProgramSentence::TERMINATOR_SEQUENCE));
        return ProgramSentence{};
    }

    if (peekSequence(ProgramSentence::CONTINUATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ProgramSentence{}, ERR(121)));
    }

    std::vector<MayFail<ProgramWord>> programWords;
    MayFail<ProgramWord> currentWord;

    LOOP until (input.peek() == EOF || peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
    if (!__first_it)
    {
        if (!consumeSequence(ProgramSentence::CONTINUATOR_SEQUENCE, input)) {
            // this happens when we have an Atom right after a non-Atom (without a space in between)
            return std::unexpected(Malformed(ProgramSentence{programWords}, ERR(102)));
        }
        if (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
            return std::unexpected(Malformed(ProgramSentence{programWords}, ERR(122)));
        }
    }
        currentWord = consumeProgramWord(input);
        programWords.push_back(currentWord);
        if (!currentWord.has_value()) {
            return std::unexpected(Malformed(ProgramSentence{programWords}, ERR(129)));
        }

        ENDLOOP
    }

    if (!consumeSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(ProgramSentence{programWords}, ERR(120)));
    }

    // eat and discard trailing newlines as well
    while (peekSequence(ProgramSentence::TERMINATOR_SEQUENCE, input)) {
        input.ignore(sequenceLen(ProgramSentence::TERMINATOR_SEQUENCE));
    }

    return ProgramSentence{programWords};
}
