#ifndef AST_VISITOR_H
#define AST_VISITOR_H

#include <monlang/ast/Program.h>
#include <monlang/ast/ProgramSentence.h>
#include <monlang/ast/Word.h>

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

#endif // AST_VISITOR_H
