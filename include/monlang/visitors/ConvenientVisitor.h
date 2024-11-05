#ifndef CONVENIENT_VISITOR_H
#define CONVENIENT_VISITOR_H

#include <monlang/visitors/visitor.h>

#include <monlang/common.h>

#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Word.h>
#include <monlang/Term.h>
#include <monlang/Word.h>

template <typename T>
class ConvenientVisitor;

template <>
class ConvenientVisitor<void> : public AstVisitor<void> {
  public:
    virtual void operator()(const MayFail<Program>&) override;
    virtual void operator()(const MayFail<ProgramSentence>&) override;
    virtual void operator()(const MayFail<ProgramWord>&) override;

    virtual void operator()(const Program&);
    virtual void operator()(const ProgramSentence&);
    virtual void operator()(const ProgramWord&);
    virtual void operator()(const Term&);
    virtual void operator()(const Word&);

    virtual void operator()(Atom*);
    virtual void operator()(SquareBracketsTerm*);
    virtual void operator()(ParenthesesGroup*);
    virtual void operator()(SquareBracketsGroup*);
    virtual void operator()(CurlyBracketsGroup*);
    virtual void operator()(PostfixParenthesesGroup*);
    virtual void operator()(PostfixSquareBracketsGroup*);
};

template <typename T>
class ConvenientVisitor : public ConvenientVisitor<void> {
  public:
    static constexpr bool returnsSomething = true;

    T res;
};

#endif // CONVENIENT_VISITOR_H
