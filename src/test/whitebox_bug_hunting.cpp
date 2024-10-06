// THIS TEST FILE SHOULD NOT BE ADDED TO `all.cpp`

#include <monlang/ProgramSentence.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/ParenthesesGroup.h>
#include <monlang/Word.h>

#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR nested malformed sbg", "[wbh-0001][wbh]") {
    auto input = "[[ ]]";

    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsGroup
       |  ~> Term
       |    ~> Word: SquareBracketsGroup
       |      ~> Term
       |        ~> ERR-131
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroupStrictly(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("multiple sbg in a sentence", "[wbh-0002][wbh]") {
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
       |  -> ProgramWord #3: SquareBracketsGroup (empty)
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgramSentence(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("multiple terms in sbg", "[wbh-0003][wbh]") {
    auto input = "[1 + 1, 2 + 2]";

    auto expect = tommy_str(R"EOF(
       |-> SquareBracketsGroup
       |  -> Term #1
       |    -> Word #1: Atom: `1`
       |    -> Word #2: Atom: `+`
       |    -> Word #3: Atom: `1`
       |  -> Term #2
       |    -> Word #1: Atom: `2`
       |    -> Word #2: Atom: `+`
       |    -> Word #3: Atom: `2`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroupStrictly(input_iss);
    auto output_word = mayfail_convert<Word>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("sbg in the middle of sbg", "[wbh-0004][wbh]") {
    auto input = "[1, [], 4]";

    auto expect = tommy_str(R"EOF(
       |-> SquareBracketsGroup
       |  -> Term #1
       |    -> Word: Atom: `1`
       |  -> Term #2
       |    -> Word: SquareBracketsGroup (empty)
       |  -> Term #3
       |    -> Word: Atom: `4`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output_word = consumeWord(input_iss);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("Term ERR trailing atom right after a non-atom", "[wbh-0005][wbh]") {
    auto input = "[[]fds]";

    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsGroup
       |  ~> Term
       |    -> Word: SquareBracketsGroup (empty)
       |    ~> ERR-103
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output_word = consumeWord(input_iss);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ProgramSentence ERR trailing atom right after a non-atom", "[wbh-0006][wbh]") {
    auto input = "[]fds";

    auto expect = tommy_str(R"EOF(
       |~> ProgramSentence
       |  -> ProgramWord: SquareBracketsGroup (empty)
       |  ~> ERR-102
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgramSentence(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("postfixes words should always be unnumbered", "[wbh-0007][wbh]") {
    auto input = "(a(b) / c)";
    auto expect = tommy_str(R"EOF(
        -> ParenthesesGroup
          -> Term
            -> Word #1: PostfixParenthesesGroup
              -> Atom: `a`
              -> ParenthesesGroup
                -> Term
                  -> Word: Atom: `b`
            -> Word #2: Atom: `/`
            -> Word #3: Atom: `c`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("postfixes words should always be unnumbered", "[wbh-0008][wbh]") {
    auto input = tommy_str(R"EOF(
        {
            b
    )EOF");
    auto expect = tommy_str(R"EOF(
        ~> ProgramSentence
          ~> ProgramWord: CurlyBracketsGroup
            ~> ProgramSentence
              -> ProgramWord: Atom: `b`
              ~> ERR-120
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgramSentence(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}
