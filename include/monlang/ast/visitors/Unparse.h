#ifndef UNPARSE_H
#define UNPARSE_H

#include <monlang/ast/visitors/ConvenientVisitor.h>

#include <sstream>

class Unparse : public ConvenientVisitor<void> {
  public:
    Unparse(std::ostream&);

    using ConvenientVisitor::operator();

    void operator()(const ProgramSentence&) override;
    void operator()(const Term&) override;
    void operator()(const Word&) override;

    void operator()(Atom*) override;
    void operator()(SquareBracketsTerm*) override;
    void operator()(ParenthesesGroup*) override;
    void operator()(SquareBracketsGroup*) override;
    void operator()(CurlyBracketsGroup*) override;

  private:
    std::ostream& out;

    int currIndent = 0;

    std::optional<Sequence> continuator = {};
    bool succeedsAContinuator = /* doesn't matter */ false;
};

#ifdef UNPARSE_H_MAIN
// #include <monlang/visitors/Unparse.h>

#include <monlang/Program.h> // consumeProgram()

#include <utils/tommystring.h>

// g++ -x c++ -D UNPARSE_H_MAIN -c include/monlang/ast/visitors/Unparse.h -o main.o --std=c++23 -Wall -Wextra -I include
// g++ main.o src/ast/visitors/Unparse.cpp src/ast/visitors/ConvenientVisitor.cpp  obj/*.o -o main.elf --std=c++23 -Wall -Wextra -I include
int main()
{
    auto input = tommy_str(R"EOF(
       |{
       |    {
       |        fds
       |    }
       |    sdf
       |}
       |
    )EOF");
    auto input_iss = std::istringstream(input);

    auto prog_ = consumeProgram(input_iss);

    if (prog_.has_error()) {
        std::cout << "wtf" << std::endl;
        return 1;
    }

    visitAst(Unparse(std::cout), (Program)prog_);
}
#endif // UNPARSE_H_MAIN

#endif // UNPARSE_H
