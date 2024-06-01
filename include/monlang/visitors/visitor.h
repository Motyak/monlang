#ifndef VISITOR_H
#define VISITOR_H

#include <monlang/common.h>
#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Word.h>
#include <monlang/Atom.h>

using Ast = std::variant<MayFail<Program>, MayFail<ProgramSentence>, MayFail<Word>>;

////////////////////////////////////////////////////////////////
// AstVisitor
////////////////////////////////////////////////////////////////

template <typename T>
class AstVisitor;

template <>
class AstVisitor<void> {
  public:
    static constexpr bool returnsSomething = false;

    virtual void operator()(const MayFail<Program>&) = 0;
    virtual void operator()(const MayFail<ProgramSentence>&) = 0;
    virtual void operator()(const MayFail<Word>&) = 0;
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

////////////////////////////////////////////////////////////////
// WordVisitor
////////////////////////////////////////////////////////////////

template <typename T>
class WordVisitor;

template <>
class WordVisitor<void> {
  public:
    static constexpr bool returnsSomething = false;

    virtual void operator()(const Atom&) = 0;
};

template <typename T>
class WordVisitor : public WordVisitor<void> {
  public:
    static constexpr bool returnsSomething = true;

    T res;
};

template <typename T>
auto visitWord(T visitor, const Word& word) {
    static_assert(std::is_base_of_v<WordVisitor<void>, T>);
    std::visit(visitor, word);
    if constexpr (visitor.returnsSomething) {
        return visitor.res;
    }
}

#endif // VISITOR_H
