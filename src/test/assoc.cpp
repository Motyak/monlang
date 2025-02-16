#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("association of two words", "[test-2111][assoc]") {
    auto input = "left:right";

    auto expect = tommy_str(R"EOF(
       |-> Association
       |  -> Word: Atom: `left`
       |  -> Word: Atom: `right`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("association of two pg", "[test-2112][assoc]") {
    auto input = "(left):(right)";

    auto expect = tommy_str(R"EOF(
       |-> Association
       |  -> Word: ParenthesesGroup
       |    -> Term
       |      -> Word: Atom: `left`
       |  -> Word: ParenthesesGroup
       |    -> Term
       |      -> Word: Atom: `right`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("atom with namespace subscript", "[test-2113][assoc]") {
    auto input = tommy_str(R"EOF(
       |fds:::sdf fds::::sdf fds:::::sdf
       |fds::sdf:aaa::bbb
       |
    )EOF");

    auto expect = tommy_str(R"EOF(
       |-> Program
       |  -> ProgramSentence #1
       |    -> ProgramWord #1: Association
       |      -> Word: Atom: `fds::`
       |      -> Word: Atom: `sdf`
       |    -> ProgramWord #2: Atom: `fds::::sdf`
       |    -> ProgramWord #3: Association
       |      -> Word: Atom: `fds::::`
       |      -> Word: Atom: `sdf`
       |  -> ProgramSentence #2
       |    -> ProgramWord: Association
       |      -> Word: Atom: `fds::sdf`
       |      -> Word: Atom: `aaa::bbb`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgram(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}
