#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

#include <monlang/Program.h>

////////////////////////////////////////////////////////////////

TEST_CASE ("empty program", "[test-1111][base]") {
    auto input = "";

    auto expect = "-> Program";

    auto input_iss = std::istringstream(input);
    auto output = consumeProgram(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("one program sentence", "[test-1112][base]") {
    auto input = tommy_str(R"EOF(
       |fds
       |
    )EOF");

    auto expect = tommy_str(R"EOF(
       |-> Program
       |  -> ProgramSentence
       |    -> ProgramWord: Atom: `fds`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgram(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("two program words", "[test-1113][base]") {
    auto input = tommy_str(R"EOF(
       |fds sdf
       |
    )EOF");

    auto expect = tommy_str(R"EOF(
       |-> Program
       |  -> ProgramSentence
       |    -> ProgramWord #1: Atom: `fds`
       |    -> ProgramWord #2: Atom: `sdf`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgram(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}
////////////////////////////////////////////////////////////////

TEST_CASE ("two program sentences", "[test-1114][base]") {
    auto input = tommy_str(R"EOF(
       |fds
       |sdf
       |
    )EOF");

    auto expect = tommy_str(R"EOF(
       |-> Program
       |  -> ProgramSentence #1
       |    -> ProgramWord: Atom: `fds`
       |  -> ProgramSentence #2
       |    -> ProgramWord: Atom: `sdf`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgram(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("two two-program-words sentences", "[test-1115][base]") {
    auto input = tommy_str(R"EOF(
       |fds sdf
       |dfs dsf
       |
    )EOF");

    auto expect = tommy_str(R"EOF(
       |-> Program
       |  -> ProgramSentence #1
       |    -> ProgramWord #1: Atom: `fds`
       |    -> ProgramWord #2: Atom: `sdf`
       |  -> ProgramSentence #2
       |    -> ProgramWord #1: Atom: `dfs`
       |    -> ProgramWord #2: Atom: `dsf`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgram(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("special atoms", "[test-1116][base]") {
    auto input = tommy_str(R"EOF(
       |:= [:] ..
       |
    )EOF");

    auto expect = tommy_str(R"EOF(
        -> ProgramSentence
          -> ProgramWord #1: Atom: `:=`
          -> ProgramWord #2: Atom: `[:]`
          -> ProgramWord #3: Atom: `..`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgramSentence(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("atom with namespace subscript", "[test-1130][base]") {
    auto input = tommy_str(R"EOF(
       |::
       |fds::sdf fds:: ::fds
       |
    )EOF");

    auto expect = tommy_str(R"EOF(
       |-> Program
       |  -> ProgramSentence #1
       |    -> ProgramWord: Atom: `::`
       |  -> ProgramSentence #2
       |    -> ProgramWord #1: Atom: `fds::sdf`
       |    -> ProgramWord #2: Atom: `fds::`
       |    -> ProgramWord #3: Atom: `::fds`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgram(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}

//==============================================================
// ERR
//==============================================================

TEST_CASE ("prog sentence hit EOF", "[test-1211][base][err]") {
    auto input = "";

    auto expect = tommy_str(R"EOF(
       |~> ProgramSentence
       |  ~> ERR-125
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgramSentence(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("atom hit EOF", "[test-1511][base][err]") {
    auto input = "";

    auto expect = tommy_str(R"EOF(
       |~> Atom: ``
       |  ~> ERR-995
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("one program sentence ERR leading continuator", "[test-1117][base][err]") {
    auto input = tommy_str(R"EOF(
       |\sfoo
       |
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> Program
       |  ~> ProgramSentence
       |    ~> ERR-121
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgram(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("one program sentence ERR trailing continuator", "[test-1118][base][err]") {
    auto input = tommy_str(R"EOF(
       |foo\s
       |
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> Program
       |  ~> ProgramSentence
       |    -> ProgramWord: Atom: `foo`
       |    ~> ERR-122
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgram(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("one program sentence ERR missing terminator", "[test-1119][base][err]") {
    auto input = tommy_str(R"EOF(
       |fds
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> Program
       |  ~> ProgramSentence
       |    -> ProgramWord: Atom: `fds`
       |    ~> ERR-120
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgram(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("atom starting as a special atom", "[test-1120][base][err]") {
    auto input = tommy_str(R"EOF(
       |:=fds
       |
    )EOF");

    auto expect = tommy_str(R"EOF(
        ~> ProgramSentence
          ~> ProgramWord: Atom: ``
            ~> ERR-992
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgramSentence(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}
