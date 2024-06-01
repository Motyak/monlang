#include <Print.h>
#include <monlang/common.h>
#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Word.h>
#include <monlang/Atom.h>

#include <utils/str-utils.h>
#include <utils/nb-utils.h>

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

    if (prog.sentences.size() > 1) {
        for (int n : range(prog.sentences.size(), 0)) {
            numbering.push(n);
        }
    } else {
        numbering.push(NO_NUMBERING);
    }

    for (auto programSentence : prog.sentences) {
        operator()(programSentence);
    }
    
    if (prog.sentences.size() > 0) {
        currentTabulation--;
    }
}

void Print::operator()(const MayFail<ProgramSentence>& programSentence) {
    ProgramSentence progSentence;
    if (programSentence.has_value()) {
        progSentence = programSentence.value();
        output("-> ");
    } else {
        progSentence = programSentence.error().val;
        output("~> ");
    }

    if (int n = numbering.top(); n == NO_NUMBERING) {
        outputLine(std::string() + "ProgramSentence");
    } else {
        outputLine(std::string() + "ProgramSentence #" + std::to_string(n));
    }
    numbering.pop();

    if (progSentence.programWords.size() > 0) {
        currentTabulation++;
    }

    if (progSentence.programWords.size() > 1) {
        for (int n : range(progSentence.programWords.size(), 0)) {
            numbering.push(n);
        }
    } else {
        numbering.push(NO_NUMBERING);
    }

    for (auto programWord: progSentence.programWords) {
        areProgramWords = true;
        operator()(mayfail_cast<Word>(programWord));
    }

    if (progSentence.programWords.size() > 0) {
        currentTabulation--;
    }
}

void Print::operator()(const MayFail<Word>& word) {
    Word word_;
    if (word.has_value()) {
        word_ = word.value();
        output("-> ");
    } else {
        word_ = word.error().val;
        output("~> ");
    }

    output(std::string() + (areProgramWords? "ProgramWord" : "Word"));
    if (int n = numbering.top(); n != NO_NUMBERING) {
        output(std::string() + " #" + std::to_string(n));
    }
    numbering.pop();
    output(": ");

    visitWord(wordVisitor, word_);
    outputLine("");
}

///////////////////////////////////////////////////////////////

void Print::_WordVisitor::operator()(const Atom& atom) {
    out << "Atom: " << "`" << atom.value << "`";
}

///////////////////////////////////////////////////////////////

Print::Print(std::ostream& os) : out(os), wordVisitor(os){}

Print::_WordVisitor::_WordVisitor(std::ostream& os) : out(os){}

void Print::output(const std::string& chars) {
    if (startOfNewLine) {
        out << std::string(currentTabulation * TAB_SIZE, SPACE);
    }
    out << chars;
    startOfNewLine = false;
}

void Print::outputLine(const std::string& line = "") {
    output(line);
    out << std::endl;
    startOfNewLine = true;
}

std::string Print::_WordVisitor::escapeTabsAndNewlines(const std::string& input) {
    auto res = input;
    res = replace_all(res, std::string(1, TAB), "\t");
    res = replace_all(res, std::string(1, NEWLINE), "\n");
    return res;
}
