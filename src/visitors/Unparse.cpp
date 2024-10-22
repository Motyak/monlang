#include <monlang/visitors/Unparse.h>
#include <monlang/visitors/ConvenientVisitor.h>

/* impl only */
#include <monlang/Atom.h>
#include <monlang/ParenthesesGroup.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/CurlyBracketsGroup.h>

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

///////////////////////////////////////////////////////////

void Unparse::operator()(Atom* atom) {
    out << atom->value;
}

void Unparse::operator()(ParenthesesGroup* pg) {
    out << ParenthesesGroup::INITIATOR_SEQUENCE;
    if (pg->terms.size() >= 2) {
        continuator = ParenthesesGroup::CONTINUATOR_SEQUENCE;
    }
    succeedsAContinuator = false;

    ConvenientVisitor::operator()(pg);

    continuator = {};
    succeedsAContinuator = false;
    out << ParenthesesGroup::TERMINATOR_SEQUENCE;
}

void Unparse::operator()(SquareBracketsGroup* sbg) {
    out << SquareBracketsGroup::INITIATOR_SEQUENCE;
    if (sbg->terms.size() >= 2) {
        continuator = SquareBracketsGroup::CONTINUATOR_SEQUENCE;
    }
    succeedsAContinuator = false;

    ConvenientVisitor::operator()(sbg);

    continuator = {};
    succeedsAContinuator = false;
    out << SquareBracketsGroup::TERMINATOR_SEQUENCE;
}

void Unparse::operator()(CurlyBracketsGroup* cbg) {
    out << CurlyBracketsGroup::INITIATOR_SEQUENCE;

    /* handle single term expression */
    if (cbg->term) {
        ConvenientVisitor::operator()(cbg->term->value());
        return;
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
