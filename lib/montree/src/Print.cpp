#include <Print.h>
#include <monlang/common.h>
#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Term.h>
#include <monlang/Word.h>
#include <monlang/Atom.h>

/* impl only */
#include <monlang/SquareBracketsGroup.h>
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

    if (numbering.empty()) {
        outputLine(std::string() + "ProgramSentence");
    } else {
        if (int n = numbering.top(); n == NO_NUMBERING) {
            outputLine(std::string() + "ProgramSentence");
        } else {
            outputLine(std::string() + "ProgramSentence #" + std::to_string(n));
        }
        numbering.pop();
    }

    if (progSentence.programWords.size() > 0 || !programSentence.has_value()) {
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

    if (!programSentence.has_value()) {
        outputLine(std::string() + "~> ERR-" + serializeErrCode(programSentence));
    }

    if (progSentence.programWords.size() > 0 || !programSentence.has_value()) {
        currentTabulation--;
    }
}

void Print::operator()(const MayFail<Word>& word) {
    curWord = word; // needed by word handlers
    Word word_;
    if (word.has_value()) {
        word_ = word.value();
        output("-> ");
    } else {
        word_ = word.error().val;
        output("~> ");
    }

    if (numbering.empty()) {
        /* then, it's a stand-alone word */
        std::visit(*this, word_);
        return;
    }

    output(std::string() + (areProgramWords? "ProgramWord" : "Word"));
    if (int n = numbering.top(); n != NO_NUMBERING) {
        output(std::string() + " #" + std::to_string(n));
    }
    numbering.pop();
    output(": ");

    std::visit(*this, word_);
    outputLine("");
}

///////////////////////////////////////////////////////////////

void Print::operator()(const SquareBracketsGroup* sbg) {
    outputLine("SquareBracketsGroup");

    if (sbg->terms.size() > 0 || !curWord.has_value()) {
        currentTabulation++;
    }

    if (sbg->terms.size() > 1) {
        for (int n : range(sbg->terms.size(), 0)) {
            numbering.push(n);
        }
    } else {
        numbering.push(NO_NUMBERING);
    }

    if (sbg->terms.size() == 0) {
        if (!curWord.has_value()) {
            outputLine(std::string() + "~> ERR-" + serializeErrCode(curWord));
        }
    } else {
        for (auto term : sbg->terms) {
            handleTerm(term);
        }
    }

    if (sbg->terms.size() > 0 || !curWord.has_value()) {
        currentTabulation--;
    }
}

void Print::operator()(const Atom& atom) {
    out << "Atom: " << "`" << atom.value << "`";
}

///////////////////////////////////////////////////////////////

Print::Print(std::ostream& os) : out(os){}

void Print::handleTerm(const MayFail<Term>& term) {
    Term term_;
    if (term.has_value()) {
        term_ = term.value();
        output("-> ");
    } else {
        term_ = term.error().val;
        output("~> ");
    }

    if (numbering.empty()) {
        outputLine(std::string() + "Term");
    } else {
        if (int n = numbering.top(); n == NO_NUMBERING) {
            outputLine(std::string() + "Term");
        } else {
            outputLine(std::string() + "Term #" + std::to_string(n));
        }
        numbering.pop();
    }

    if (term_.words.size() > 0 || !term.has_value()) {
        currentTabulation++;
    }

    if (term_.words.size() > 1) {
        for (int n : range(term_.words.size(), 0)) {
            numbering.push(n);
        }
    } else {
        numbering.push(NO_NUMBERING);
    }

    for (auto word: term_.words) {
        areProgramWords = false;
        operator()(mayfail_cast<Word>(word));
    }

    if (!term.has_value()) {
        outputLine(std::string() + "~> ERR-" + serializeErrCode(term));
    }

    if (term_.words.size() > 0 || !term.has_value()) {
        currentTabulation--;
    }
}

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
