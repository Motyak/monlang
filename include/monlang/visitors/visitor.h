#ifndef VISITOR_H
#define VISITOR_H

#include <monlang/ast/visitors/visitor.h>

#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Word.h>
#include <monlang/Term.h>

using Ast_ = std::variant<
    MayFail<MayFail_<Program>>,
    MayFail<MayFail_<ProgramSentence>>,
    MayFail<ProgramWord_>,
    MayFail<MayFail_<Term>>,
    MayFail<Word_>
>;

template <typename T>
class AstVisitor_;

template <>
class AstVisitor_<void> {
  public:
    static constexpr bool returnsSomething = false;

    virtual void operator()(const MayFail<MayFail_<Program>>&) = 0;
    virtual void operator()(const MayFail<MayFail_<ProgramSentence>>&) = 0;
    virtual void operator()(const MayFail<ProgramWord_>&) = 0;
    virtual void operator()(const MayFail<MayFail_<Term>>&) = 0;
    virtual void operator()(const MayFail<Word_>&) = 0;
};

template <typename T>
class AstVisitor_ : public AstVisitor_<void> {
  public:
    static constexpr bool returnsSomething = true;

    T res;
};

template <typename T>
auto visitAst_(T visitor, const Ast_& tree) {
    static_assert(std::is_base_of_v<AstVisitor_<void>, T>);
    std::visit(visitor, tree);
    if constexpr (visitor.returnsSomething) {
        return visitor.res;
    }
}

#endif // VISITOR_H
