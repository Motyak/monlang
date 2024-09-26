// THIS TEST FILE SHOULD NOT BE ADDED TO `all.cpp`

#include <monlang/Word.h>

#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("fds") {
    auto input = tommy_str(R"EOF(
        [{1 + 2};]
    )EOF");
    auto expect = "whatever";

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}
