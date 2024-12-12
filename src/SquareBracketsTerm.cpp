#include <monlang/SquareBracketsTerm.h>

const Sequence SquareBracketsTerm::INITIATOR_SEQUENCE = {'[', SPACE};
const Sequence SquareBracketsTerm::TERMINATOR_SEQUENCE = {SPACE, ']'};

MayFail<MayFail_<SquareBracketsTerm>> consumeSquareBracketsTerm(std::istringstream& input) {
    RECORD_INPUT_STREAM_PROGRESS();
    TRACE_CUR_FUN();
    const Sequence termTerminatorSeq = SquareBracketsTerm::TERMINATOR_SEQUENCE;

    if (!consumeSequence(SquareBracketsTerm::INITIATOR_SEQUENCE, input)) {
        return Malformed(MayFail_<SquareBracketsTerm>{}, ERR(052));
    }

    // shouldn't be empty
    if (peekSequence(SquareBracketsTerm::TERMINATOR_SEQUENCE, input)) {
        return Malformed(MayFail_<SquareBracketsTerm>{}, ERR(521));
    }

    MayFail<MayFail_<Term>> term = consumeTerm(termTerminatorSeq, input);
    if (term.has_error()) {
        return Malformed(MayFail_<SquareBracketsTerm>{term}, ERR(529));
    }

    if (!consumeSequence(SquareBracketsTerm::TERMINATOR_SEQUENCE, input)) {
        return Malformed(MayFail_<SquareBracketsTerm>{term}, ERR(520));
    }

    auto sbt = MayFail_<SquareBracketsTerm>{term};
    sbt._tokenLen = GET_INPUT_STREAM_PROGRESS();
    return sbt;
}

///////////////////////////////////////////////////////////

SquareBracketsTerm::SquareBracketsTerm(Term term) : term(term){}

MayFail_<SquareBracketsTerm>::MayFail_(MayFail<MayFail_<Term>> term) : term(term){}

MayFail_<SquareBracketsTerm>::MayFail_(SquareBracketsTerm sbt) {
    this->term = MayFail_<Term>(sbt.term);
}

MayFail_<SquareBracketsTerm>::operator SquareBracketsTerm() const {
    return SquareBracketsTerm{(Term)term.value()};
}
