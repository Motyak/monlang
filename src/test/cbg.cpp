#include <monlang/CurlyBracketsGroup.h>
#include <monlang/Word.h>

#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("empty curly brackets group", "[test-4111][cbg]") {
    auto input = "{}";

    auto expect = "-> CurlyBracketsGroup (empty)";

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(0, input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}
