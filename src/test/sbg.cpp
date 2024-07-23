#include <monlang/SquareBracketsGroup.h>
#include <monlang/Word.h>

#include <utils/mem-utils.h>
#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("empty square brackets group", "[test-4311][base]") {
    auto input = "[]";
    
    auto expect = "-> SquareBracketsGroup";

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}
