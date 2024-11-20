#include <monlang/ast/visitors/Unparse.h>
#include <monlang/ast/visitors/ConvenientVisitor.h>

/* impl only */
#include <monlang/ast/Atom.h>
#include <monlang/ast/SquareBracketsTerm.h>
#include <monlang/ast/ParenthesesGroup.h>
#include <monlang/ast/SquareBracketsGroup.h>
#include <monlang/ast/CurlyBracketsGroup.h>

Unparse::Unparse(std::ostream& out) : out(out) {}

void Unparse::operator()(const ProgramSentence& programSentence) {
    out << (currIndent * ProgramSentence::TAB_SEQUENCE);

    if (programSentence.programWords.size() >= 2) {
        continuator = ProgramSentence::CONTINUATOR_SEQUENCE;
    }
    succeedsAContinuator = false;

    ConvenientVisitor::operator()(programSentence);

    continuator = {};
    succeedsAContinuator = false;
    out << ProgramSentence::TERMINATOR_SEQUENCE;
}

void Unparse::operator()(const Term& term) {
    if (term.words.size() >= 2) {
        continuator = Term::CONTINUATOR_SEQUENCE;
    }

    ConvenientVisitor::operator()(term);

    continuator = {};
    succeedsAContinuator = false;
}

void Unparse::operator()(const Word& word) {
    if (continuator) {
        if (succeedsAContinuator) {
            out << continuator.value();
        } else {
            succeedsAContinuator = true;
        }
    }

    ConvenientVisitor::operator()(word);
}

void Unparse::operator()(const ProgramWord& pw) {
    if (continuator) {
        if (succeedsAContinuator) {
            out << continuator.value();
        } else {
            succeedsAContinuator = true;
        }
    }

    ConvenientVisitor::operator()(pw);
}

///////////////////////////////////////////////////////////

void Unparse::operator()(Atom* atom) {
    out << atom->value;
}

void Unparse::operator()(ParenthesesGroup* pg) {
    auto saved_ContinuatorSeq = continuator; // backup curr continuator
    auto saved_SucceedsAContinuator = succeedsAContinuator; // backup curr `succeeds a continuator`

    out << ParenthesesGroup::INITIATOR_SEQUENCE;
    if (pg->terms.size() >= 2) {
        continuator = ParenthesesGroup::CONTINUATOR_SEQUENCE;
    }
    succeedsAContinuator = false;

    ConvenientVisitor::operator()(pg);

    continuator = saved_ContinuatorSeq;
    succeedsAContinuator = saved_SucceedsAContinuator;

    out << ParenthesesGroup::TERMINATOR_SEQUENCE;
}

void Unparse::operator()(SquareBracketsTerm* sbt) {
    auto saved_ContinuatorSeq = continuator; // backup curr continuator
    auto saved_SucceedsAContinuator = succeedsAContinuator; // backup curr `succeeds a continuator`

    out << SquareBracketsTerm::INITIATOR_SEQUENCE;
    succeedsAContinuator = false;

    ConvenientVisitor::operator()(sbt);

    continuator = saved_ContinuatorSeq;
    succeedsAContinuator = saved_SucceedsAContinuator;

    out << SquareBracketsTerm::TERMINATOR_SEQUENCE;
}

void Unparse::operator()(SquareBracketsGroup* sbg) {
    auto saved_ContinuatorSeq = continuator; // backup curr continuator
    auto saved_SucceedsAContinuator = succeedsAContinuator; // backup curr `succeeds a continuator`

    out << SquareBracketsGroup::INITIATOR_SEQUENCE;
    if (sbg->terms.size() >= 2) {
        continuator = SquareBracketsGroup::CONTINUATOR_SEQUENCE;
    }
    succeedsAContinuator = false;

    ConvenientVisitor::operator()(sbg);

    continuator = saved_ContinuatorSeq;
    succeedsAContinuator = saved_SucceedsAContinuator;

    out << SquareBracketsGroup::TERMINATOR_SEQUENCE;
}

void Unparse::operator()(CurlyBracketsGroup* cbg) {
    out << CurlyBracketsGroup::INITIATOR_SEQUENCE;

    /* handle single term expression */
    if (cbg->term) {
        ConvenientVisitor::operator()(*cbg->term);
    }
    /* handle multiline expression */
    else {
        out << ProgramSentence::TERMINATOR_SEQUENCE;
        currIndent++;
        ConvenientVisitor::operator()(cbg);
        currIndent--;
        out << (currIndent * ProgramSentence::TAB_SEQUENCE);
    }

    out << CurlyBracketsGroup::TERMINATOR_SEQUENCE;
}
