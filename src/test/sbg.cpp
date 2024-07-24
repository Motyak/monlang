#include <monlang/SquareBracketsGroup.h>
#include <monlang/Word.h>

#include <utils/mem-utils.h>
#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("empty square brackets group", "[test-4311][sbg]") {
    auto input = "[]";
    
    auto expect = "-> SquareBracketsGroup";

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroup(input_iss);
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
    auto output = consumeSquareBracketsGroup(input_iss);
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
    auto output = consumeSquareBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("one two-words term in square brackets group", "[test-4313][sbg]") {
    auto input = "[fds sdf]";
    
    auto expect = tommy_str(R"EOF(
       |-> SquareBracketsGroup
       |  -> Term
       |    -> Word #1: Atom: `fds`
       |    -> Word #2: Atom: `sdf`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("two two-words terms in square brackets group", "[test-4313][sbg]") {
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
    auto output = consumeSquareBracketsGroup(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}
