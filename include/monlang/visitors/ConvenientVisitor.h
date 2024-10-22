#ifndef CONVENIENT_VISITOR_H
#define CONVENIENT_VISITOR_H

#include <monlang/visitors/visitor.h>

#include <monlang/common.h>

#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/ProgramWord.h>
#include <monlang/Term.h>
#include <monlang/Word.h>

template <typename T>
class ConvenientVisitor;

template <>
class ConvenientVisitor<void> : public AstVisitor<void> {
  public:
    virtual void operator()(const MayFail<Program>& prog) override;
    virtual void operator()(const MayFail<ProgramSentence>& progSentence) override;
    virtual void operator()(const MayFail<Word>& word) override;

    virtual void operator()(const Program& prog);
    virtual void operator()(const ProgramSentence& progSentence);
    virtual void operator()(const ProgramWord& progWord);
    virtual void operator()(const Term& term);
    virtual void operator()(const Word& word);

    virtual void operator()(Atom*);
    virtual void operator()(ParenthesesGroup* pg);
    virtual void operator()(SquareBracketsGroup* sbg);
    virtual void operator()(CurlyBracketsGroup* cbg);
    virtual void operator()(PostfixParenthesesGroup* ppg);
    virtual void operator()(PostfixSquareBracketsGroup* psbg);
};

template <typename T>
class ConvenientVisitor : public ConvenientVisitor<void> {
  public:
    static constexpr bool returnsSomething = true;

    T res;
};

#endif // CONVENIENT_VISITOR_H
