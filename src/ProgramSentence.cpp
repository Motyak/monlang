#include <ProgramSentence.h>
#include <common.h>

const std::vector<char> ProgramSentence::TERMINATOR_SEQUENCE = { NEWLINE };

const std::vector<char> ProgramSentence::RESERVED_CHARACTERS = {
    ProgramSentence::TERMINATOR_SEQUENCE[0]
};
