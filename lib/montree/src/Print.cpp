#include <Print.h>
#include <monlang/common.h>
#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Word.h>
#include <monlang/Atom.h>

#include <utils/str-utils.h>
#include <regex>

Print::Print(std::ostream& os) : out(os), wordVisitor(os){}

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

    if (prog.sentences.size() > 0) {
        currentTabulation++;
    }
    for (auto sentence: prog.sentences) {
        operator()(sentence);
    }
}

void Print::operator()(const MayFail<ProgramSentence>& programSentence) {
    ProgramSentence progSentence;
    if (programSentence.has_value()) {
        progSentence = programSentence.value();
        outputLine("-> ProgramSentence");
    } else {
        progSentence = programSentence.error().val;
        outputLine("~> ProgramSentence");
    }
    
    if (progSentence.programWords.size() > 0) {
        currentTabulation++;
    }
    for (auto programWord: progSentence.programWords) {
        areProgramWords = true;
        operator()(mayfail_cast<Word>(programWord));
    }
}

void Print::operator()(const MayFail<Word>& word) {
    Word word_;
    if (word.has_value()) {
        word_ = word.value();
        output(std::string() + "-> " + (areProgramWords? "ProgramWord" : "Word") + ": ");
    } else {
        word_ = word.error().val;
        output(std::string() + "~> " + (areProgramWords? "ProgramWord" : "Word") + ": ");
    }

    visitWord(wordVisitor, word_);
}

void Print::output(const std::string& chars) {
    if (startOfNewLine) {
        out << std::string(currentTabulation * TAB_SIZE, SPACE);
    }
    out << chars;
    startOfNewLine = false;
}

void Print::outputLine(const std::string& line) {
    output(line);
    out << std::endl;
    startOfNewLine = true;
}

Print::_WordVisitor::_WordVisitor(std::ostream& os) : out(os){}

void Print::_WordVisitor::operator()(const Atom& atom) {
    out << "Atom: " << "`" << escapeTabsAndNewlines(atom.value) << "`";
}

std::string Print::_WordVisitor::escapeTabsAndNewlines(const std::string& input) {
    auto res = input;
    res = replace_all(res, std::string(1, TAB), "\t");
    res = replace_all(res, std::string(1, NEWLINE), "\n");
    return res;
}
