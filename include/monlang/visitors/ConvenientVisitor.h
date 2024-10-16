#ifndef SIMPLE_VISITOR_H
#define SIMPLE_VISITOR_H

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
            operator()(word);
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

#ifdef SIMPLE_VISITOR_H_MAIN
// #include <monlang/visitors/ConvenientVisitor.h>
#include <monlang/Atom.h>

struct Fds : public ConvenientVisitor<void> {

    using ConvenientVisitor::operator();

    void operator()(PostfixSquareBracketsGroup* psbg) override {
        std::cout << "PostfixSBG: " << std::endl;
        ConvenientVisitor::operator()(psbg);
    }

    void operator()(Atom* atom) override {
        std::cout << "Atom: `" << atom->value << "`" << std::endl;
    }

    void operator()(SquareBracketsGroup* sbg) override {
        std::cout << "SBG: ";
        ConvenientVisitor::operator()(sbg);
    }
};

/*
g++ -o obj/main.o -x c++ -c include/monlang/visitors/ConvenientVisitor.h -D SIMPLE_VISITOR_H_MAIN --std=c++23 -Wall -Wextra -I include \
&& g++ -o bin/main.elf obj/*.o
*/
int main()
{
    auto input = std::istringstream("foo[bar]\n");
    auto composable_atom = consumeWord(input);
    if (!composable_atom.has_value()) {
        std::cout << "wtf" << std::endl;
        return 0;
    }
    visitAst(Fds{}, composable_atom);
}
#endif // SIMPLE_VISITOR_H_MAIN

#endif // SIMPLE_VISITOR_H
