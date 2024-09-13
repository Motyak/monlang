// THIS TEST FILE SHOULD NOT BE ADDED TO `all.cpp`

#include <monlang/Word.h>

#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("fds") {
    // auto input = "[[2 + 2, 3 + 3], [4 + 4]]";
    // auto input = "[1, [2 + 2, 3 + 3], 4]";
    auto input = "[1, (), 4]";

    // auto input = "[1 + 1, (2 + 2, 3 + 3), (4 + 4), (), 5]";
    // auto input = "[1 + 1, 5]";
    // auto input = "foo";

    auto expect = "whatever";

    auto input_iss = std::istringstream(input);
    auto output_word = consumeWord(input_iss);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}
