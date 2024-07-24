#ifndef PRINT_H
#define PRINT_H

#include <monlang/common.h>
#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Word.h>

#include <monlang/Atom.h>
#include <monlang/Term.h>

/* interface only */
#include <monlang/visitors/visitor.h>
#include <stack>

class Print : public AstVisitor<void> {
  public:
    Print(std::ostream&);
    void operator()(const MayFail<Program>&) override;
    void operator()(const MayFail<ProgramSentence>&) override;
    void operator()(const MayFail<Word>&) override;

    void operator()(const SquareBracketsGroup*);
    void operator()(const Atom&);

  private:
    static constexpr int TAB_SIZE = 2;
    static constexpr int NO_NUMBERING = -1;
    
    void handleTerm(const MayFail<Term>&);
    void output(const std::string&);
    void outputLine(const std::string&);

    std::ostream& out;
    std::stack<int> numbering;
    bool startOfNewLine = true;
    unsigned currentTabulation = 0;
    bool areProgramWords = false;
};

#endif // PRINT_H
