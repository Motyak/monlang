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
