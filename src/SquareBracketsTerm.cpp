#include <monlang/SquareBracketsTerm.h>

const Sequence SquareBracketsTerm::INITIATOR_SEQUENCE = {'[', SPACE};
const Sequence SquareBracketsTerm::TERMINATOR_SEQUENCE = {SPACE, ']'};

MayFail<MayFail_<SquareBracketsTerm>> consumeSquareBracketsTerm(std::istringstream& input) {
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

    return MayFail_<SquareBracketsTerm>{term};
}

///////////////////////////////////////////////////////////

MayFail_<SquareBracketsTerm> SquareBracketsTerm::wrap() const {
    return MayFail_<SquareBracketsTerm>{MayFail_<Term>(term)};
}

MayFail_<SquareBracketsTerm>::MayFail_(MayFail<MayFail_<Term>> term) : term(term){}

MayFail_<SquareBracketsTerm>::MayFail_(SquareBracketsTerm sbt) : MayFail_(sbt.wrap()){}

MayFail_<SquareBracketsTerm>::operator SquareBracketsTerm() const {
    return SquareBracketsTerm{(Term)term.value()};
}

SquareBracketsTerm MayFail_<SquareBracketsTerm>::unwrap() const {
    return (SquareBracketsTerm)*this;
}
