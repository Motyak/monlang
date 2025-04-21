#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

#include <monlang/MultilineSquareBracketsGroup.h>
#include <monlang/Word.h>

#include <utils/mem-utils.h>

////////////////////////////////////////////////////////////////

TEST_CASE ("multiline square brackets group", "[test-4400][msbg]") {
    auto input = tommy_str(R"EOF(
       |[
       |    fds
       |    sdf
       |]
    )EOF");

    auto expect = tommy_str(R"EOF(
       |-> MultilineSquareBracketsGroup
       |  -> ProgramSentence #1
       |    -> ProgramWord: Atom: `fds`
       |  -> ProgramSentence #2
       |    -> ProgramWord: Atom: `sdf`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeMultilineSquareBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

//==============================================================
// ERR
//==============================================================

TEST_CASE ("Malformed MultilineSquareBracketsGroup, expect indented/empty newline after initiator", "[test-4401][msbg][err]") {
    auto input = tommy_str(R"EOF(
       |[
       |]
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> MultilineSquareBracketsGroup
       |  ~> ERR-441
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeMultilineSquareBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("Malformed MultilineSquareBracketsGroup, must contain at least one sentence (encountered block with empty lines exclusively)", "[test-4402][msbg][err]") {
    auto input = tommy_str(R"EOF(
       |[
       |
       |\s\s\s\s
       |]
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> MultilineSquareBracketsGroup
       |  ~> ERR-442
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeMultilineSquareBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR prog sentence wrong indent", "[test-4403][msbg][err]") {
    auto input = tommy_str(R"EOF(
       |[
       |  wrong
       |]
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> MultilineSquareBracketsGroup
       |  ~> ProgramSentence
       |    ~> ERR-123
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeMultilineSquareBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR cbg missing terminator", "[test-4404][msbg][err]") {
    auto input = tommy_str(R"EOF(
       |[
       |    fds
       |
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> MultilineSquareBracketsGroup
       |  -> ProgramSentence
       |    -> ProgramWord: Atom: `fds`
       |  ~> ERR-440
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeMultilineSquareBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}
