#include <monlang/ProgramSentence.h>
#include <monlang/Word.h>
#include <monlang/CurlyBracketsGroup.h>

#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("nested curly brackets group", "[test-1000][int]") {
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

////////////////////////////////////////////////////////////////

TEST_CASE ("postfix pg", "[test-1001][int]") {
    auto input = "fds(a)(b)";

    auto expect = tommy_str(R"EOF(
       |-> PostfixParenthesesGroup
       |  -> PostfixParenthesesGroup
       |    -> Atom: `fds`
       |    -> ParenthesesGroup
       |      -> Term
       |        -> Word: Atom: `a`
       |  -> ParenthesesGroup
       |    -> Term
       |      -> Word: Atom: `b`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output_word = consumeWord(input_iss);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

//==============================================================
// ERR
//==============================================================

TEST_CASE ("ProgramSentence ERR trailing atom right after a non-atom", "[test-1002][int][err]") {
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
