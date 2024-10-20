#ifndef UNPARSE_H
#define UNPARSE_H

#include <monlang/visitors/ConvenientVisitor.h>

class Unparse : public ConvenientVisitor<void> {
  public:
    Unparse(std::ostream&);

    using ConvenientVisitor::operator();

    void operator()(const ProgramSentence&) override;
    void operator()(const Word&) override;

    void operator()(Atom*) override;
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

#include <utils/tommystring.h>

// g++ -o main.o -x c++ -c include/monlang/visitors/Unparse.h -D UNPARSE_H_MAIN --std=c++23 -Wall -Wextra -I include
// g++ --std=c++23 -o main.elf main.o src/visitors/Unparse.cpp obj/*.o -I include
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

    auto prog = consumeProgram(input_iss);

    if (!prog.has_value()) {
        std::cout << "wtf" << std::endl;
        return 1;
    }

    visitAst(Unparse(std::cout), prog);
}
#endif // UNPARSE_H_MAIN

#endif // UNPARSE_H
