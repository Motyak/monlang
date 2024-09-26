#include <monlang/Word.h>

#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("postfix sbg", "[test-3211][int][postfix]") {
    auto input = "fds[]";

    auto expect = tommy_str(R"EOF(
       |-> PostfixSquareBracketsGroup
       |  -> Atom: `fds`
       |  -> SquareBracketsGroup (empty)
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output_word = consumeWord(input_iss);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}
