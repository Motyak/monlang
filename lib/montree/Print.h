#ifndef PRINT_H
#define PRINT_H

#include <monlang/common.h>
#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Word.h>
#include <monlang/Atom.h>

#include <monlang/visitors/visitor.h> // interface only
#include <stack>

class Print : public AstVisitor<void> {
  public:
    Print(std::ostream&);
    void operator()(const MayFail<Program>&);
    void operator()(const MayFail<ProgramSentence>&);
    void operator()(const MayFail<Word>&);

  private:
    static constexpr int TAB_SIZE = 2;
    static constexpr int NO_NUMBERING = -1;

    void output(const std::string&);
    void outputLine(const std::string&);

    std::stack<int> numbering;
    bool startOfNewLine = true;
    unsigned currentTabulation = 0;
    bool areProgramWords = false;
    std::ostream& out;

    class _WordVisitor : public WordVisitor<void> {
      public:
        std::ostream& out;
        _WordVisitor(std::ostream&);
        void operator()(const Atom&);

      private:
        std::string escapeTabsAndNewlines(const std::string&);
    } wordVisitor;
};

#endif // PRINT_H
