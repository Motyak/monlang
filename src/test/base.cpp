
#include <monlang/Program.h>
#include <utils/tommystring.h>

#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

TEST_CASE ("empty program", "[test-1000][base]") {
    auto input = "";
    
    auto expect = "-> Program";

    auto input_iss = std::istringstream(input);
    auto output = consumeProgram(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}
