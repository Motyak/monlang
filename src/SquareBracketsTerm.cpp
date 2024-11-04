#include <monlang/SquareBracketsTerm.h>

const Sequence SquareBracketsTerm::INITIATOR_SEQUENCE = {'[', SPACE};
const Sequence SquareBracketsTerm::TERMINATOR_SEQUENCE = {SPACE, ']'};

const std::vector<char> SquareBracketsTerm::RESERVED_CHARACTERS = {
    sequenceFirstChar(INITIATOR_SEQUENCE).value(),
    sequenceFirstChar(TERMINATOR_SEQUENCE).value(),
};

MayFail<SquareBracketsTerm> consumeSquareBracketsTerm(std::istringstream& input) {
    TRACE_CUR_FUN();
    const std::vector<char> termTerminatorChars = {
        sequenceFirstChar(SquareBracketsTerm::TERMINATOR_SEQUENCE).value()
    };

    if (!consumeSequence(SquareBracketsTerm::INITIATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(SquareBracketsTerm{}, ERR(052)));
    }

    // shouldn't be empty
    if (peekSequence(SquareBracketsTerm::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(SquareBracketsTerm{}, ERR(521)));
    }

    MayFail<Term> term = consumeTerm(termTerminatorChars, input);
    if (!term.has_value()) {
        return std::unexpected(Malformed(SquareBracketsTerm{term}, ERR(529)));
    }

    if (!consumeSequence(SquareBracketsTerm::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(SquareBracketsTerm{term}, ERR(520)));
    }

    return SquareBracketsTerm{term};
}
