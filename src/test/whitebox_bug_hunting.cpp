// THIS TEST FILE SHOULD NOT BE PART OF THE TEST SUITE (all.elf)

/*
What it tests:
- hidden features (behavior that could be "derived"/extrapolated..
                 ..from the features but is not introduced explicitly)
- non-regression for bugs found during exploration in the playground.cpp..
                        ..then fixed
*/

#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

#include <monlang/ProgramSentence.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/ParenthesesGroup.h>
#include <monlang/Word.h>

////////////////////////////////////////////////////////////////

TEST_CASE ("empty ProgramSentences == empty Program as well", "[wbh-0020][wbh]") {
    auto input = tommy_str(R"EOF(
       |
       |
    )EOF");

    auto expect = "-> Program";

    auto input_iss = std::istringstream(input);
    auto output = consumeProgram(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("ERR nested malformed sbg", "[wbh-0001][wbh]") {
    auto input = "[[,]]";

    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsGroup
       |  ~> Term
       |    ~> Word: SquareBracketsGroup
       |      ~> Term
       |        ~> Word: Atom: ``
       |          ~> ERR-992
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroupStrictly(input_iss);
    auto output_word = mayfail_convert<ProgramWord_>(output);
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
    auto output_word = mayfail_convert<ProgramWord_>(output);
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
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("postfixes words should always be unnumbered", "[wbh-0007][wbh]") {
    auto input = "(a(b) / c)";
    auto expect = tommy_str(R"EOF(
        -> ParenthesesGroup
          -> Term
            -> Word #1: PostfixParenthesesGroup
              -> Word: Atom: `a`
              -> ParenthesesGroup
                -> Term
                  -> Word: Atom: `b`
            -> Word #2: Atom: `/`
            -> Word #3: Atom: `c`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("should not print programsentence error when any malformed program words", "[wbh-0008][wbh]") {
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

////////////////////////////////////////////////////////////////

TEST_CASE ("discard in-between indented empty lines in cbg", "[wbh-0009][wbh]") {
    auto input = tommy_str(R"EOF(
       |{
       |    fds
       |\s\s\s\s
       |    sdf
       |}
    )EOF");
    auto expect = tommy_str(R"EOF(
        -> CurlyBracketsGroup
          -> ProgramSentence #1
            -> ProgramWord: Atom: `fds`
          -> ProgramSentence #2
            -> ProgramWord: Atom: `sdf`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("discard in-between non-indented empty lines in cbg", "[wbh-0010][wbh]") {
    auto input = tommy_str(R"EOF(
       |{
       |    fds
       |
       |    sdf
       |}
    )EOF");
    auto expect = tommy_str(R"EOF(
        -> CurlyBracketsGroup
          -> ProgramSentence #1
            -> ProgramWord: Atom: `fds`
          -> ProgramSentence #2
            -> ProgramWord: Atom: `sdf`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("postfix propagate err", "[wbh-0011][wbh]") {
    auto input = "fds( )";

    auto expect = tommy_str(R"EOF(
       |~> PostfixParenthesesGroup
       |  -> Word: Atom: `fds`
       |  ~> ParenthesesGroup
       |    ~> Term
       |      ~> ERR-131
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("association propagate err", "[wbh-0012][wbh]") {
    auto input = "fds:( )";

    auto expect = tommy_str(R"EOF(
       |~> Association
       |  -> Word: Atom: `fds`
       |  ~> Word: ParenthesesGroup
       |    ~> Term
       |      ~> ERR-131
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}
