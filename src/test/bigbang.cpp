#include <monlang/Program.h>

#include <utils/tommystring.h>
#include <utils/file-utils.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("<catch2 section wrapper>", "[bigbang]") {

    static constexpr int TAB_SIZE = 4;
    static const std::string DIR = "src/test/bigbang_data";

    SECTION ("do..until loop with inner if") {
        auto input = tommy_str(R"EOF(
           |do {
           |    let curr next(&stream)
           |    unless [ $first_it ] {
           |        print(SPACE)
           |    }
           |    print(curr)
           |}
           |until [ eof?(stream) ]
           |
        )EOF");

        auto expect = slurp_file(DIR + "/test-1100.expected.txt");

        auto input_iss = std::istringstream(input);
        auto output = consumeProgram(input_iss);
        auto output_str = montree::astToString(output, TAB_SIZE);
        REQUIRE (output_str == expect);
    }

    // more sections to come...

}
