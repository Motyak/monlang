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

TEST_CASE ("single word curly brackets term", "[test-4112][cbg]") {
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

TEST_CASE ("two words curly brackets term", "[test-4113][cbg]") {
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

//==============================================================
// ERR
//==============================================================

TEST_CASE ("ERR cbg missing initiator", "[test-4115][cbg][err]") {
    auto input = "";

    auto expect = tommy_str(R"EOF(
       |~> CurlyBracketsGroup
       |  ~> ERR-041
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR curly brackets term missing terminator", "[test-4116][cbg][err]") {
    auto input = "{fds";

    auto expect = tommy_str(R"EOF(
       |~> CurlyBracketsGroup
       |  -> Term
       |    -> Word: Atom: `fds`
       |  ~> ERR-510
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR curly brackets term leading space", "[test-4141][cbg][err]") {
    auto input = "{ fds}";

    auto expect = tommy_str(R"EOF(
       |~> CurlyBracketsGroup
       |  ~> Term
       |    ~> ERR-131
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR curly brackets term trailing space", "[test-4142][cbg][err]") {
    auto input = "{fds }";

    auto expect = tommy_str(R"EOF(
       |~> CurlyBracketsGroup
       |  ~> Term
       |    -> Word #1: Atom: `fds`
       |    ~> Word #2: Atom: ``
       |      ~> ERR-992
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR cbg missing indented newline after initiator", "[test-4117][cbg][err]") {
    auto input = tommy_str(R"EOF(
       |{
       |}
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> CurlyBracketsGroup
       |  ~> ERR-412
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR multiline cbg must contain at least one sentence (empty lines)", "[test-4155][cbg][err]") {
    auto input = tommy_str(R"EOF(
       |{
       |
       |\s\s\s\s
       |}
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> CurlyBracketsGroup
       |  ~> ERR-413
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR prog sentence wrong indent", "[test-4118][cbg][err]") {
    auto input = tommy_str(R"EOF(
       |{
       |  wrong
       |}
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> CurlyBracketsGroup
       |  ~> ProgramSentence
       |    ~> ERR-123
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR cbg missing terminator", "[test-4119][cbg][err]") {
    auto input = tommy_str(R"EOF(
       |{
       |    fds
       |
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> CurlyBracketsGroup
       |  -> ProgramSentence
       |    -> ProgramWord: Atom: `fds`
       |  ~> ERR-410
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}
