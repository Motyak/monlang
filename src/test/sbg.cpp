#include <monlang/SquareBracketsGroup.h>
#include <monlang/Word.h>

#include <utils/mem-utils.h>
#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("empty square brackets group", "[test-4311][sbg]") {
    auto input = "[]";

    auto expect = "-> SquareBracketsGroup (empty)";

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroupStrictly(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("one one-word term in square brackets group", "[test-4312][sbg]") {
    auto input = "[fds]";
    
    auto expect = tommy_str(R"EOF(
       |-> SquareBracketsGroup
       |  -> Term
       |    -> Word: Atom: `fds`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroupStrictly(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("two one-word terms in square brackets group", "[test-4313][sbg]") {
    auto input = "[fds, sdf]";
    
    auto expect = tommy_str(R"EOF(
       |-> SquareBracketsGroup
       |  -> Term #1
       |    -> Word: Atom: `fds`
       |  -> Term #2
       |    -> Word: Atom: `sdf`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroupStrictly(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("one two-words term in square brackets group", "[test-4314][sbg]") {
    auto input = "[fds sdf]";
    
    auto expect = tommy_str(R"EOF(
       |-> SquareBracketsGroup
       |  -> Term
       |    -> Word #1: Atom: `fds`
       |    -> Word #2: Atom: `sdf`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroupStrictly(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("two two-words terms in square brackets group", "[test-4315][sbg]") {
    auto input = "[fds sdf, abc def]";
    
    auto expect = tommy_str(R"EOF(
       |-> SquareBracketsGroup
       |  -> Term #1
       |    -> Word #1: Atom: `fds`
       |    -> Word #2: Atom: `sdf`
       |  -> Term #2
       |    -> Word #1: Atom: `abc`
       |    -> Word #2: Atom: `def`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroupStrictly(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("postfix sbg", "[test-4361][sbg]") {
    auto input = "fds[]";

    auto expect = tommy_str(R"EOF(
       |-> PostfixSquareBracketsGroup
       |  -> Atom: `fds`
       |  -> SquareBracketsGroup (empty)
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output_word = consumeWord(input_iss);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR missing sbg initiator", "[test-4316][sbg][err]") {
    auto input = "";
    
    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsGroup
       |  ~> ERR-043
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroupStrictly(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR premature EOF", "[test-4317][sbg][err]") {
    auto input = "[";

    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsGroup
       |  ~> Term
       |    ~> ERR-135
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroupStrictly(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR empty term", "[test-4318][sbg][err]") {
    auto input = "[,]";

    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsGroup
       |  ~> Term
       |    ~> Word: Atom: ``
       |      ~> ERR-992
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroupStrictly(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR trailing continuator in term", "[test-4319][sbg][err]") {
    auto input = "[fds ]";

    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsGroup
       |  ~> Term
       |    -> Word #1: Atom: `fds`
       |    ~> Word #2: Atom: ``
       |      ~> ERR-992
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroupStrictly(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR missing continuator", "[test-4320][sbg][err]") {
    auto input = "[fds,]";

    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsGroup
       |  -> Term
       |    -> Word: Atom: `fds`
       |  ~> ERR-403
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroupStrictly(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR empty second term", "[test-4321][sbg][err]") {
    auto input = "[fds, ]";

    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsGroup
       |  -> Term #1
       |    -> Word: Atom: `fds`
       |  ~> Term #2
       |    ~> Word: Atom: ``
       |      ~> ERR-992
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroupStrictly(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR heading term continuator", "[test-4322][sbg][err]") {
    auto input = "[ fds]";

    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsGroup
       |  ~> Term
       |    ~> ERR-131
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroupStrictly(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}
