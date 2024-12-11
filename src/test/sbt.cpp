#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

#include <monlang/SquareBracketsTerm.h>
#include <monlang/Word.h>

////////////////////////////////////////////////////////////////

TEST_CASE ("one word term in square brackets term", "[test-5211][sbt]") {
    auto input = "[ fds ]";

    auto expect = tommy_str(R"EOF(
       |-> SquareBracketsTerm
       |  -> Term
       |    -> Word: Atom: `fds`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsTerm(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("two words term in square brackets term", "[test-5212][sbt]") {
    auto input = "[ fds sdf ]";

    auto expect = tommy_str(R"EOF(
       |-> SquareBracketsTerm
       |  -> Term
       |    -> Word #1: Atom: `fds`
       |    -> Word #2: Atom: `sdf`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsTerm(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

//==============================================================
// ERR
//==============================================================

TEST_CASE ("Term ERR hit EOF", "[test-5213][sbt][err]") {
    auto input = "";

    auto input_iss = std::istringstream(input);
    auto output = consumeTerm(input_iss);
    REQUIRE (output.has_error());
    REQUIRE (output.error().fmt == "ERR-135");
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR missing sbt initiator", "[test-5214][sbt][err]") {
    auto input = "";

    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsTerm
       |  ~> ERR-052
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsTerm(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR empty sbt (single space)", "[test-5236][sbt][err]") {
    auto input = tommy_str(R"EOF(
        [\s]
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsTerm
       |  ~> Term
       |    ~> Word: Atom: ``
       |      ~> ERR-992
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsTerm(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR empty sbt", "[test-5216][sbt][err]") {
    auto input = tommy_str(R"EOF(
        [\s\s]
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsTerm
       |  ~> ERR-521
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsTerm(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR leading term continuator", "[test-5217][sbt][err]") {
    auto input = tommy_str(R"EOF(
        [ \sfds ]
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsTerm
       |  ~> Term
       |    ~> ERR-131
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsTerm(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR trailing term continuator", "[test-5218][sbt][err]") {
    auto input = tommy_str(R"EOF(
        [ fds\s ]
    )EOF");

    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsTerm
       |  ~> Term
       |    -> Word #1: Atom: `fds`
       |    ~> Word #2: Atom: ``
       |      ~> ERR-992
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsTerm(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}
