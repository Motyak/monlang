// THIS TEST FILE SHOULD NOT BE PART OF THE TEST SUITE (all.elf)

#include <monlang/Word.h>

#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("fds") {
    auto input = tommy_str(R"EOF(
       |{
       |    if [ blabla ] {
       |        A
       |    }
       |
       |    else {
       |        B
       |    }
       |}
    )EOF");

    auto expect = "whatever";

    auto input_iss = std::istringstream(input);
    auto output_word = consumeProgramWord(input_iss);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}
