#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

#include <monlang/ProgramSentence.h>
#include <monlang/Word.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/CurlyBracketsGroup.h>

////////////////////////////////////////////////////////////////

TEST_CASE ("nested square brackets group", "[test-1000][int]") {
    auto input = "[[fds], sdf]";

    auto expect = tommy_str(R"EOF(
       |-> SquareBracketsGroup
       |  -> Term #1
       |    -> Word: SquareBracketsGroup
       |      -> Term
       |        -> Word: Atom: `fds`
       |  -> Term #2
       |    -> Word: Atom: `sdf`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("nested curly brackets group", "[test-1001][int]") {
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
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("nested postfix parentheses group", "[test-1002][int]") {
    auto input = "fds(a)(b)";

    auto expect = tommy_str(R"EOF(
       |-> PostfixParenthesesGroup
       |  -> Word: PostfixParenthesesGroup
       |    -> Word: Atom: `fds`
       |    -> ParenthesesGroup
       |      -> Term
       |        -> Word: Atom: `a`
       |  -> ParenthesesGroup
       |    -> Term
       |      -> Word: Atom: `b`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("nested association", "[test-1051][int]") {
    auto input = "a:b:c";

    auto expect = tommy_str(R"EOF(
       |-> Association
       |  -> Word: Atom: `a`
       |  -> Word: Association
       |    -> Word: Atom: `b`
       |    -> Word: Atom: `c`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("all words in a same program sentence", "[test-1003][int]") {
    auto input = "atom (pg) {cbg} ppg() a:b\n";

    auto expect = tommy_str(R"EOF(
       |-> ProgramSentence
       |  -> ProgramWord #1: Atom: `atom`
       |  -> ProgramWord #2: ParenthesesGroup
       |    -> Term
       |      -> Word: Atom: `pg`
       |  -> ProgramWord #3: CurlyBracketsGroup
       |    -> Term
       |      -> Word: Atom: `cbg`
       |  -> ProgramWord #4: PostfixParenthesesGroup
       |    -> Word: Atom: `ppg`
       |    -> ParenthesesGroup (empty)
       |  -> ProgramWord #5: Association
       |    -> Word: Atom: `a`
       |    -> Word: Atom: `b`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeProgramSentence(input_iss);
    auto output_str = montree::astToString(output);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("all words in a same term", "[test-1004][int]") {
    auto input = "[atom (pg) {cbg} ppg() a:b]";

    auto expect = tommy_str(R"EOF(
       |-> SquareBracketsGroup
       |  -> Term
       |    -> Word #1: Atom: `atom`
       |    -> Word #2: ParenthesesGroup
       |      -> Term
       |        -> Word: Atom: `pg`
       |    -> Word #3: CurlyBracketsGroup
       |      -> Term
       |        -> Word: Atom: `cbg`
       |    -> Word #4: PostfixParenthesesGroup
       |      -> Word: Atom: `ppg`
       |      -> ParenthesesGroup (empty)
       |    -> Word #5: Association
       |      -> Word: Atom: `a`
       |      -> Word: Atom: `b`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
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

////////////////////////////////////////////////////////////////

TEST_CASE ("Term ERR trailing atom right after a non-atom", "[test-1003][int][err]") {
    auto input = "[[]fds]";

    auto expect = tommy_str(R"EOF(
       |~> SquareBracketsGroup
       |  ~> Term
       |    -> Word: SquareBracketsGroup (empty)
       |    ~> ERR-103
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}
