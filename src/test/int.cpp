#include <monlang/ProgramSentence.h>

#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("multiple sbg in a sentence", "[test-1511][int]") {
    auto input = tommy_str(R"EOF(
       |[a, b] [c] []
       |
    )EOF");
    
    auto expect = tommy_str(R"EOF(
       |-> ProgramSentence
       |  -> ProgramWord #1: SquareBracketsGroup
       |    -> Term #1
       |      -> Word: Atom: `a`
       |    -> Term #2
       |      -> Word: Atom: `b`
       |  -> ProgramWord #2: SquareBracketsGroup
       |    -> Term
       |      -> Word: Atom: `c`
       |  -> ProgramWord #3: SquareBracketsGroup
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgramSentence(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}
