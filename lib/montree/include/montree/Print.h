#ifndef PRINT_H
#define PRINT_H

#include <monlang/common.h>
#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Word.h>

#include <monlang/Term.h>

/* interface only */
#include <monlang/visitors/visitor.h>
#include <stack>

class Print : public AstVisitor<void> {
  public:
    Print(std::ostream&, int TAB_SIZE=2);

    void operator()(const MayFail<Program>&) override;
    void operator()(const MayFail<ProgramSentence>&) override;
    void operator()(const MayFail<Word>&) override;

    void operator()(SquareBracketsTerm*);
    void operator()(SquareBracketsGroup*);
    void operator()(ParenthesesGroup*);
    void operator()(CurlyBracketsGroup*);
    void operator()(Atom*);
    void operator()(PostfixSquareBracketsGroup*);
    void operator()(PostfixParenthesesGroup*);

    void operator()(auto); // fall-through

  private:
    static constexpr int NO_NUMBERING = -1;
    const int TAB_SIZE;

    void handleTerm(const MayFail<Term>&);
    void output(const char* strs...);
    void outputLine(const char* strs...);

    std::ostream& out;
    std::stack<int> numbering;
    bool startOfNewLine = true;
    int currIndent = 0;
    bool areProgramWords = false;
    MayFail<Word> curWord;
};

#endif // PRINT_H
