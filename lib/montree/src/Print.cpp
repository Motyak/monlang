#include <Print.h>

Print::Print(std::ostream& os) : firstLine(true), currentTabulation(0), out(os), wordVisitor(os){}

void Print::operator()(const MayFail<Program>& program) {
    // TODO: use std::expected monadic operations
    Program prog;
    if (program.has_value()) {
        prog = program.value();
        outputLine("-> Program");
    } else {
        prog = program.error().val;
        outputLine("~> Program");
    }

    for (auto sentence: prog.sentences) {
        operator()(sentence);
    }
}

void Print::operator()(const MayFail<ProgramSentence>& programSentence) {
    //TODO
    out << "program sentence";
}

void Print::operator()(const MayFail<Word>& word) {
    //TODO
    visitWord(wordVisitor, /*tmp ugly fix*/ *word);
}

void Print::outputLine(const std::string& line) {
    if (firstLine) {
        firstLine = false;
    } else {
        out << std::endl;
    }
    out << std::string(currentTabulation, SPACE) << line;
}

Print::_WordVisitor::_WordVisitor(std::ostream& os) : out(os){}

void Print::_WordVisitor::operator()(const Atom& atom) {
    //TODO
    out << "atom";
}
