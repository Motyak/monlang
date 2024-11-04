#include <monlang/SquareBracketsTerm.h>

const Sequence SquareBracketsTerm::INITIATOR_SEQUENCE = {'[', SPACE};
const Sequence SquareBracketsTerm::TERMINATOR_SEQUENCE = {SPACE, ']'};

MayFail<SquareBracketsTerm> consumeSquareBracketsTerm(std::istringstream& input) {
    TRACE_CUR_FUN();
    const Sequence termTerminatorSeq = SquareBracketsTerm::TERMINATOR_SEQUENCE;

    if (!consumeSequence(SquareBracketsTerm::INITIATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(SquareBracketsTerm{}, ERR(052)));
    }

    // shouldn't be empty
    if (peekSequence(SquareBracketsTerm::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(SquareBracketsTerm{}, ERR(521)));
    }

    MayFail<Term> term = consumeTerm(termTerminatorSeq, input);
    if (!term.has_value()) {
        return std::unexpected(Malformed(SquareBracketsTerm{term}, ERR(529)));
    }

    if (!consumeSequence(SquareBracketsTerm::TERMINATOR_SEQUENCE, input)) {
        return std::unexpected(Malformed(SquareBracketsTerm{term}, ERR(520)));
    }

    return SquareBracketsTerm{term};
}
