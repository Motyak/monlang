#include <utils/tommystring.h>
#include <utils/file-utils.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

TEST_CASE ("cast", "[test-3000][cast]") {
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

    auto expect = slurp_file("src/test/bigbang_data/test-1100.expected.txt");

    auto input_iss = std::istringstream(input);
    auto output1 = consumeProgram(input_iss);
    auto output2 = (Program)output1;
    auto output3 = MayFail(MayFail_<Program>(output2));
    auto output_str = montree::astToString(output3, /*TAB_SIZE*/4);
    REQUIRE (output_str == expect);
}
