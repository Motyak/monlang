#ifndef VISITOR_H
#define VISITOR_H

#include <monlang/common.h>
#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Word.h>

using Ast = std::variant<
    MayFail<MayFail_<Program>>,
    MayFail<MayFail_<ProgramSentence>>,
    MayFail<ProgramWord_>
>;

template <typename T>
class AstVisitor;

template <>
class AstVisitor<void> {
  public:
    static constexpr bool returnsSomething = false;

    virtual void operator()(const MayFail<MayFail_<Program>>&) = 0;
    virtual void operator()(const MayFail<MayFail_<ProgramSentence>>&) = 0;
    virtual void operator()(const MayFail<ProgramWord_>& word) = 0;
};

template <typename T>
class AstVisitor : public AstVisitor<void> {
  public:
    static constexpr bool returnsSomething = true;

    T res;
};

template <typename T>
auto visitAst(T visitor, const Ast& tree) {
    static_assert(std::is_base_of_v<AstVisitor<void>, T>);
    std::visit(visitor, tree);
    if constexpr (visitor.returnsSomething) {
        return visitor.res;
    }
}

#endif // VISITOR_H
