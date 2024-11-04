#include <montree/Print.h>

#include <monlang/common.h>
#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Term.h>
#include <monlang/Word.h>

/* impl only */
#include <monlang/Atom.h>
#include <monlang/SquareBracketsTerm.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/ParenthesesGroup.h>
#include <monlang/CurlyBracketsGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>
#include <monlang/PostfixParenthesesGroup.h>

#include <utils/nb-utils.h>
#include <utils/str-utils.h>
#include <cstdarg>

#define until(x) while(!(x))

#define SERIALIZE_ERR(x) serializeErr(x).c_str()

#define outputLine(...) \
    output(__VA_ARGS__ __VA_OPT__(,) "\n", nullptr); \
    startOfNewLine = true

#define output(a, ...) output(a, __VA_ARGS__ __VA_OPT__(,) nullptr)
void (Print::output)(const char* strs...) {
    if (startOfNewLine) {
        out << std::string(currIndent * TAB_SIZE, SPACE);
    }

    va_list args;
    va_start(args, strs);
    const char* currArg = strs;
    until (currArg == nullptr) {
        out << currArg;
        currArg = va_arg(args, const char*);
    }
    va_end(args);

    startOfNewLine = false;
}

///////////////////////////////////////////////////////////////

void Print::operator()(const MayFail<Program>& program) {
    const Program& prog = mayfail_unwrap(program);
    outputLine(program.has_value()? "-> Program" : "~> Program");

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
    const ProgramSentence& progSentence = mayfail_unwrap(programSentence);
    output(programSentence.has_value()? "-> " : "~> ");

    if (numbering.empty()) {
        outputLine("ProgramSentence");
    } else {
        if (int n = numbering.top(); n == NO_NUMBERING) {
            outputLine("ProgramSentence");
        } else {
            outputLine("ProgramSentence #", itoa(n));
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

    int malformedProgramWords = 0;
    for (auto programWord: progSentence.programWords) {
        areProgramWords = true;
        operator()(mayfail_cast<Word>(programWord));
        if (!programWord.has_value()) {
            malformedProgramWords += 1;
        }
    }

    if (!programSentence.has_value() && malformedProgramWords == 0) {
        outputLine("~> ", SERIALIZE_ERR(programSentence));
    }

    // note: should always enter since a ProgramSentence cannot be empty
    if (progSentence.programWords.size() > 0 || !programSentence.has_value()) {
        currIndent--;
    }
}

void Print::operator()(const MayFail<Word>& word) {
    this->curWord = word; // needed by word handlers
    const Word& word_ = mayfail_unwrap(word);
    output(word.has_value()? "-> " : "~> ");

    if (numbering.empty()) {
        /* then, it's a stand-alone word */
        std::visit(*this, word_);
        return;
    }

    output(areProgramWords? "ProgramWord" : "Word");
    if (int n = numbering.top(); n != NO_NUMBERING) {
        output(" #", itoa(n));
    }
    numbering.pop();
    output(": ");

    std::visit(*this, word_); // in case of malformed word,...
                              // ...will still print its partial value
}

///////////////////////////////////////////////////////////////

void Print::operator()(SquareBracketsTerm* sbt) {
    auto curWord_ = curWord; // backup because it gets overriden by `handleTerm`..
                             // ..(which calls operator()(Word))

    outputLine("SquareBracketsTerm");
    currIndent++;

    if (!curWord_.has_value() && sbt->term.has_value()) {
        outputLine("~> ", SERIALIZE_ERR(curWord_));
        return;
    }

    numbering.push(NO_NUMBERING);
    handleTerm(sbt->term);

    currIndent--;
}

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
        outputLine("~> ", SERIALIZE_ERR(curWord_));
    } else {
        int nb_of_malformed_terms = 0;
        for (auto term : sbg->terms) {
            if (!term.has_value()) {
                nb_of_malformed_terms++;
            }
            handleTerm(term);
        }
        if (nb_of_malformed_terms == 0 && !curWord_.has_value()) {
            outputLine("~> ", SERIALIZE_ERR(curWord_));
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
        outputLine("~> ", SERIALIZE_ERR(curWord_));
    } else {
        int nb_of_malformed_terms = 0;
        for (auto term : pg->terms) {
            if (!term.has_value()) {
                nb_of_malformed_terms++;
            }
            handleTerm(term);
        }
        if (nb_of_malformed_terms == 0 && !curWord_.has_value()) {
            outputLine("~> ", SERIALIZE_ERR(curWord_));
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
        numbering.push(NO_NUMBERING);
        handleTerm(term);
        if (term.has_value() && !curWord_.has_value()) {
            outputLine("~> ", SERIALIZE_ERR(curWord_));
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
        outputLine("~> ", SERIALIZE_ERR(curWord_));
    } else {
        int nb_of_malformed_sentences = 0;
        for (auto sentence : cbg->sentences) {
            if (!sentence.has_value()) {
                nb_of_malformed_sentences++;
            }
            operator()(sentence);
        }
        if (nb_of_malformed_sentences == 0 && !curWord_.has_value()) {
            outputLine("~> ", SERIALIZE_ERR(curWord_));
        }
    }

    currIndent--;
}

void Print::operator()(Atom* atom) {
    outputLine("Atom: `", atom->value.c_str(), "`");
    if (!curWord.has_value()) {
        currIndent++;
        outputLine("~> ", SERIALIZE_ERR(curWord));
        currIndent--;
    }
}

void Print::operator()(PostfixSquareBracketsGroup* psbg) {
    outputLine("PostfixSquareBracketsGroup");

    auto savedStack = numbering;
    /* add `Word: ` prefix in tree */
    numbering = std::stack<int>({NO_NUMBERING});
    areProgramWords = false;

    currIndent++;
    operator()(MayFail<Word>(psbg->leftPart));
    currIndent--;

    currIndent++;
    operator()(MayFail<Word>(&mayfail_unwrap(psbg->rightPart)));
    currIndent--;

    numbering = savedStack;
}

void Print::operator()(PostfixParenthesesGroup* ppg) {
    outputLine("PostfixParenthesesGroup");

    auto savedStack = numbering;
    /* add `Word: ` prefix in tree */
    numbering = std::stack<int>({NO_NUMBERING});
    areProgramWords = false;

    currIndent++;
    operator()(MayFail<Word>(ppg->leftPart));
    currIndent--;

    currIndent++;
    operator()(MayFail<Word>(&mayfail_unwrap(ppg->rightPart)));
    currIndent--;

    numbering = savedStack;
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
        outputLine("Term");
    } else {
        if (int n = numbering.top(); n == NO_NUMBERING) {
            outputLine("Term");
        } else {
            outputLine("Term #", itoa(n));
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
        outputLine("~> ", SERIALIZE_ERR(term));
    }

    if (term_.words.size() > 0 || !term.has_value()) {
        currIndent--;
    }
}
