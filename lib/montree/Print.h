#ifndef PRINT_H
#define PRINT_H

#include <monlang/visitors/visitor.h>
#include <monlang/common.h>
#include <monlang/Program.h>
#include <monlang/ProgramSentence.h>
#include <monlang/Word.h>
#include <monlang/Atom.h>

class Print : public AstVisitor<void> {
  public:
    Print(std::ostream&);
    void operator()(const MayFail<Program>&);
    void operator()(const MayFail<ProgramSentence>&);
    void operator()(const MayFail<Word>&);

  private:
    void outputLine(const std::string&);

    bool firstLine;
    unsigned currentTabulation;
    std::ostream& out;

    struct _WordVisitor : public WordVisitor<void> {
        std::ostream& out;
        _WordVisitor(std::ostream&);
        void operator()(const Atom&);
    } wordVisitor;
};

#endif // PRINT_H
