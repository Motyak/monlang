#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

#include <monlang/Quotation.h>

////////////////////////////////////////////////////////////////

TEST_CASE ("empty (singleline) quotation", "[test-5311][quot]") {
    auto input = tommy_str(R"EOF(
        ""
    )EOF");

    auto expect = "-> Quotation: ``";

    auto input_iss = std::istringstream(input);
    auto output = consumeQuotation(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("singleline quotation", "[test-5312][quot]") {
    auto input = tommy_str(R"EOF(
        "line"
    )EOF");

    auto expect = "-> Quotation: `line`";

    auto input_iss = std::istringstream(input);
    auto output = consumeQuotation(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("singleline quotation with escape sequences", "[test-5312][quot]") {
    auto input = tommy_str(R"EOF(
        "line1\n\"line2\""
    )EOF");

    auto expect = tommy_str(R"EOF(
       |-> Quotation: `line1` (1 more line)
       |  -> `"line2"`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeQuotation(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("multiline quotation", "[test-5313][quot]") {
    auto input = tommy_str(R"EOF(
       |```
       |    line1
       |    "line2"
       |```
    )EOF");

    auto expect = tommy_str(R"EOF(
       |-> Quotation: `line1` (1 more line)
       |  -> `"line2"`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeMultilineQuotation(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("empty multiline quotation", "[test-5314][quot]") {
    auto input = tommy_str(R"EOF(
       |```
       |
       |```
    )EOF");

    auto expect = tommy_str(R"EOF(
       |-> Quotation: ``
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeMultilineQuotation(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("trailing backslash in multiline quotation", "[test-5315][quot]") {
    auto input = tommy_str(R"EOF(
       |```
       |    \
       |```
    )EOF");

    auto expect = tommy_str(R"EOF(
       |-> Quotation: ``
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeMultilineQuotation(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

//==============================================================
// ERR
//==============================================================

TEST_CASE ("ERR MultilineQuotation expect indented/empty newline after initiator", "[test-5331][quot][err]") {
    auto input = tommy_str(R"EOF(
       |```
       |```
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> Quotation: ``
       |  ~> ERR-541
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeMultilineQuotation(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR MultilineQuotation line has wrong indentation", "[test-5332][quot][err]") {
    auto input = tommy_str(R"EOF(
       |```
       |wrong
       |```
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> Quotation: ``
       |  ~> ERR-542
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeMultilineQuotation(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}
