#include <monlang/CurlyBracketsGroup.h>
#include <monlang/Word.h>

#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("empty curly brackets group", "[test-4111][cbg]") {
    auto input = "{}";

    auto expect = "-> CurlyBracketsGroup (empty)";

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("single word curly bracket term", "[test-4112][cbg]") {
    auto input = "{single}";

    auto expect = tommy_str(R"EOF(
       |-> CurlyBracketsGroup
       |  -> Term
       |    -> Word: Atom: `single`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("curly bracket term", "[test-4113][cbg]") {
    auto input = "{fds sdf}";

    auto expect = tommy_str(R"EOF(
       |-> CurlyBracketsGroup
       |  -> Term
       |    -> Word #1: Atom: `fds`
       |    -> Word #2: Atom: `sdf`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("curly brackets group", "[test-4114][cbg]") {
    auto input = tommy_str(R"EOF(
       |{
       |    fds
       |    sdf
       |}
    )EOF");

    auto expect = tommy_str(R"EOF(
       |-> CurlyBracketsGroup
       |  -> ProgramSentence #1
       |    -> ProgramWord: Atom: `fds`
       |  -> ProgramSentence #2
       |    -> ProgramWord: Atom: `sdf`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("nested curly brackets group", "[test-4115][cbg]") {
    auto input = tommy_str(R"EOF(
       |{
       |    {
       |        fds
       |    }
       |    sdf
       |}
    )EOF");

    auto expect = tommy_str(R"EOF(
       |-> CurlyBracketsGroup
       |  -> ProgramSentence #1
       |    -> ProgramWord: CurlyBracketsGroup
       |      -> ProgramSentence
       |        -> ProgramWord: Atom: `fds`
       |  -> ProgramSentence #2
       |    -> ProgramWord: Atom: `sdf`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}
