#ifndef CONVENIENT_VISITOR_H
#define CONVENIENT_VISITOR_H

#include <monlang/visitors/visitor.h>

#include <monlang/common.h>

#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/ProgramWord.h>
#include <monlang/Term.h>
#include <monlang/Word.h>

#include <monlang/ParenthesesGroup.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/CurlyBracketsGroup.h>

#include <monlang/PostfixParenthesesGroup.h>
#include <monlang/PostfixSquareBracketsGroup.h>

#include <utils/mem-utils.h>

template <typename T>
class ConvenientVisitor;

template <>
class ConvenientVisitor<void> : public AstVisitor<void> {
  public:
    virtual void operator()(const MayFail<Program>& prog) override {
        operator()(prog.value());
    }

    virtual void operator()(const MayFail<ProgramSentence>& progSentence) override {
        operator()(progSentence.value());
    }

    virtual void operator()(const MayFail<Word>& word) override {
        operator()(word.value());
    }

    ////////////////////////////////////////////////////////////////

    virtual void operator()(const Program& prog) {
        for (auto progSentence: prog.sentences) {
            operator()(progSentence.value());
        }
    }

    virtual void operator()(const ProgramSentence& progSentence) {
        for (auto progWord: progSentence.programWords) {
            operator()(progWord.value());
        }
    }

    virtual void operator()(const ProgramWord& progWord){
        operator()(static_cast<Word>(progWord));
    }

    virtual void operator()(const Term& term) {
        for (auto word: term.words) {
            operator()(word.value());
        }
    }

    virtual void operator()(const Word& word) {
        std::visit(*this, word);
    }

    ////////////////////////////////////////////////////////////////

    virtual void operator()(Atom*){} // operation-specific

    virtual void operator()(ParenthesesGroup* pg) {
        for (auto term: pg->terms) {
            operator()(term.value());
        }
    }

    virtual void operator()(SquareBracketsGroup* sbg) {
        for (auto term: sbg->terms) {
            operator()(term.value());
        }
    }

    virtual void operator()(CurlyBracketsGroup* cbg) {
        for (auto sentence: cbg->sentences) {
            operator()(sentence.value());
        }
    }

    virtual void operator()(PostfixParenthesesGroup* ppg) {
        operator()(ppg->leftPart);
        operator()(move_to_heap(ppg->rightPart.value()));
    }

    virtual void operator()(PostfixSquareBracketsGroup* psbg) {
        operator()(psbg->leftPart);
        operator()(move_to_heap(psbg->rightPart.value()));
    }
};

template <typename T>
class ConvenientVisitor : public ConvenientVisitor<void> {
  public:
    static constexpr bool returnsSomething = true;

    T res;
};

#endif // CONVENIENT_VISITOR_H
