#ifndef CONVENIENT_VISITOR_H
#define CONVENIENT_VISITOR_H

#include <monlang/ast/visitors/visitor.h>

#include <monlang/ast/Program.h>
#include <monlang/ast/ProgramSentence.h>
#include <monlang/ast/Word.h>
#include <monlang/ast/Term.h>
#include <monlang/ast/Word.h>

template <typename T>
class ConvenientVisitor;

template <>
class ConvenientVisitor<void> : public AstVisitor<void> {
  public:
    virtual void operator()(const Program&) override;
    virtual void operator()(const ProgramSentence&) override;
    virtual void operator()(const ProgramWord&) override;
    virtual void operator()(const Term&) override;
    virtual void operator()(const Word&) override;

    virtual void operator()(Atom*);
    virtual void operator()(Quotation*);
    virtual void operator()(SquareBracketsTerm*);
    virtual void operator()(ParenthesesGroup*);
    virtual void operator()(SquareBracketsGroup*);
    virtual void operator()(CurlyBracketsGroup*);
    virtual void operator()(PostfixParenthesesGroup*);
    virtual void operator()(PostfixSquareBracketsGroup*);
    virtual void operator()(Association*);
};

template <typename T>
class ConvenientVisitor : public ConvenientVisitor<void> {
  public:
    static constexpr bool returnsSomething = true;

    T res;
};

#endif // CONVENIENT_VISITOR_H
