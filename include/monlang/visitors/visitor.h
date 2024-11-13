#ifndef VISITOR_H
#define VISITOR_H

#include <monlang/common.h>
#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Word.h>

using Ast = std::variant<Program, ProgramSentence, ProgramWord>;

template <typename T>
class AstVisitor;

template <>
class AstVisitor<void> {
  public:
    static constexpr bool returnsSomething = false;

    virtual void operator()(const Program&) = 0;
    virtual void operator()(const ProgramSentence&) = 0;
    virtual void operator()(const ProgramWord& word) = 0;
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

///////////////////////////////////////////////////////////

using Ast_ = std::variant<
    MayFail<MayFail_<Program>>,
    MayFail<MayFail_<ProgramSentence>>,
    MayFail<ProgramWord_>
>;

template <typename T>
class AstVisitor_;

template <>
class AstVisitor_<void> {
  public:
    static constexpr bool returnsSomething = false;

    virtual void operator()(const MayFail<MayFail_<Program>>&) = 0;
    virtual void operator()(const MayFail<MayFail_<ProgramSentence>>&) = 0;
    virtual void operator()(const MayFail<ProgramWord_>& word) = 0;
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
