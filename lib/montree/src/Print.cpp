#include <montree/Print.h>

#include <monlang/common.h>
#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Term.h>
#include <monlang/Word.h>

/* impl only */
#include <monlang/Atom.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/ParenthesesGroup.h>
#include <monlang/CurlyBracketsGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>
#include <monlang/PostfixParenthesesGroup.h>
#include <utils/nb-utils.h>

void Print::operator()(const MayFail<Program>& program) {
    Program prog;
    if (program.has_value()) {
        prog = program.value();
        outputLine("-> Program");
    } else {
        prog = program.error().val;
        outputLine("~> Program");
    }

    if (prog.sentences.size() > 0) {
        currIndent++;
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
        currIndent--;
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

    // note: should always enter since a ProgramSentence cannot be empty
    if (progSentence.programWords.size() > 0 || !programSentence.has_value()) {
        currIndent++;
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

    // note: should always enter since a ProgramSentence cannot be empty
    if (progSentence.programWords.size() > 0 || !programSentence.has_value()) {
        currIndent--;
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

    std::visit(*this, word_); // in case of malformed word,...
                              // ...will still print its partial value
}

///////////////////////////////////////////////////////////////

void Print::operator()(SquareBracketsGroup* sbg) {
    auto curWord_ = curWord; // backup because it gets overriden by `handleTerm`..
                             // ..(which calls operator()(Word))

    output("SquareBracketsGroup");
    if (sbg->terms.size() == 0 && curWord_.has_value()) {
        outputLine(" (empty)");
        return;
    }
    outputLine();

    currIndent++;

    if (sbg->terms.size() > 1) {
        for (int n : range(sbg->terms.size(), 0)) {
            numbering.push(n);
        }
    } else {
        numbering.push(NO_NUMBERING);
    }

    if (sbg->terms.size() == 0) {
        ASSERT(!curWord_.has_value());
        outputLine(std::string() + "~> ERR-" + serializeErrCode(curWord_));
    } else {
        int nb_of_malformed_terms = 0;
        for (auto term : sbg->terms) {
            if (!term.has_value()) {
                nb_of_malformed_terms++;
            }
            handleTerm(term);
        }
        if (nb_of_malformed_terms == 0 && !curWord_.has_value()) {
            outputLine(std::string() + "~> ERR-" + serializeErrCode(curWord_));
        }
    }

    currIndent--;
}

void Print::operator()(ParenthesesGroup* pg) {
    auto curWord_ = curWord; // backup because it gets overriden by `handleTerm`..
                             // ..(which calls operator()(Word))

    output("ParenthesesGroup");
    if (pg->terms.size() == 0 && curWord_.has_value()) {
        outputLine(" (empty)");
        return;
    }
    outputLine();

    currIndent++;

    if (pg->terms.size() > 1) {
        for (int n : range(pg->terms.size(), 0)) {
            numbering.push(n);
        }
    } else {
        numbering.push(NO_NUMBERING);
    }

    if (pg->terms.size() == 0) {
        ASSERT(!curWord_.has_value());
        outputLine(std::string() + "~> ERR-" + serializeErrCode(curWord_));
    } else {
        int nb_of_malformed_terms = 0;
        for (auto term : pg->terms) {
            if (!term.has_value()) {
                nb_of_malformed_terms++;
            }
            handleTerm(term);
        }
        if (nb_of_malformed_terms == 0 && !curWord_.has_value()) {
            outputLine(std::string() + "~> ERR-" + serializeErrCode(curWord_));
        }
    }

    currIndent--;
}

void Print::operator()(CurlyBracketsGroup* cbg) {
    auto curWord_ = curWord; // backup because it gets overriden by `handleTerm`..
                             // ..(which calls operator()(Word))

    output("CurlyBracketsGroup");

    if (cbg->sentences.size() == 0 && curWord_.has_value()) {
        outputLine(" (empty)");
        return;
    }
    outputLine();

    currIndent++;

    /* handle single term */
    if (cbg->term) {
        auto term = cbg->term.value();
        handleTerm(term);
        if (!curWord_.has_value()) {
            outputLine(std::string() + "~> ERR-" + serializeErrCode(curWord_));
        }
        currIndent--;
        return;
    }

    if (cbg->sentences.size() > 1) {
        for (int n : range(cbg->sentences.size(), 0)) {
            numbering.push(n);
        }
    } else {
        numbering.push(NO_NUMBERING);
    }

    if (cbg->sentences.size() == 0) {
        ASSERT(!curWord_.has_value());
        outputLine(std::string() + "~> ERR-" + serializeErrCode(curWord_));
    } else {
        int nb_of_malformed_sentences = 0;
        for (auto sentence : cbg->sentences) {
            if (!sentence.has_value()) {
                nb_of_malformed_sentences++;
            }
            operator()(sentence);
        }
        if (nb_of_malformed_sentences == 0 && !curWord_.has_value()) {
            outputLine(std::string() + "~> ERR-" + serializeErrCode(curWord_));
        }
    }

    currIndent--;
}

void Print::operator()(Atom* atom) {
    outputLine(std::string() + "Atom: `" + atom->value + "`");
    if (!curWord.has_value()) {
        currIndent++;
        outputLine(std::string() + "~> ERR-" + serializeErrCode(curWord));
        currIndent--;
    }
}

void Print::operator()(PostfixSquareBracketsGroup* psbg) {
    outputLine("PostfixSquareBracketsGroup");

    currIndent++;
    operator()(MayFail<Word>(psbg->leftPart));
    currIndent--;

    currIndent++;
    operator()(mayfail_convert<Word>(psbg->rightPart));
    currIndent--;
}

void Print::operator()(PostfixParenthesesGroup* ppg) {
    outputLine("PostfixParenthesesGroup");

    currIndent++;
    operator()(MayFail<Word>(ppg->leftPart));
    currIndent--;

    currIndent++;
    operator()(mayfail_convert<Word>(ppg->rightPart));
    currIndent--;
}

void Print::operator()(auto) {
    outputLine("<ENTITY NOT IMPLEMENTED YET>");
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
        currIndent++;
    }

    if (term_.words.size() > 1) {
        for (int n : range(term_.words.size(), 0)) {
            numbering.push(n);
        }
    } else {
        numbering.push(NO_NUMBERING);
    }

    int nb_of_malformed_words = 0;
    for (auto word: term_.words) {
        areProgramWords = false;
        if (!word.has_value()) {
            nb_of_malformed_words++;
        }
        operator()(mayfail_cast<Word>(word));
    }

    if (nb_of_malformed_words == 0 && !term.has_value()) {
        outputLine(std::string() + "~> ERR-" + serializeErrCode(term));
    }

    if (term_.words.size() > 0 || !term.has_value()) {
        currIndent--;
    }
}

void Print::output(const std::string& chars) {
    if (startOfNewLine) {
        out << std::string(currIndent * TAB_SIZE, SPACE);
    }
    out << chars;
    startOfNewLine = false;
}

void Print::outputLine(const std::string& line) {
    output(line);
    out << std::endl;
    startOfNewLine = true;
}
