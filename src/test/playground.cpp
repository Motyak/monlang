// THIS TEST FILE SHOULD NOT BE PART OF THE TEST SUITE (all.elf)

#include <monlang/Word.h>

#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("fds") {
    // auto input = tommy_str(R"EOF(
    //    |{
    //    |    let foo {
    //    |        bar
    //    |    }
    //    |    fds
    //    |}
    // )EOF");
    auto input = "{ +let_foo_{ ++bar +} +fds } ";

    auto expect = tommy_str(R"EOF(
        -> CurlyBracketsGroup
          -> ProgramSentence #1
            -> ProgramWord #1: Atom: `let`
            -> ProgramWord #2: Atom: `foo`
            -> ProgramWord #3: CurlyBracketsGroup
              -> ProgramSentence
                -> ProgramWord: Atom: `bar`
          -> ProgramSentence #2
            -> ProgramWord: Atom: `fds`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output_word = consumeWord(input_iss);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}
