#include <monlang/Word.h>
#include <monlang/SquareBracketsGroup.h>
#include <monlang/ParenthesesGroup.h>
#include <monlang/CurlyBracketsGroup.h>

#include <utils/tommystring.h>
#include <montree/montree.h>
#include <catch2/catch_amalgamated.hpp>

////////////////////////////////////////////////////////////////

TEST_CASE ("postfix pg off of Atom", "[test-3311][postfix]") {
    auto input = "func()";

    auto expect = tommy_str(R"EOF(
       |-> PostfixParenthesesGroup
       |  -> Word: Atom: `func`
       |  -> ParenthesesGroup (empty)
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("postfix sbg off of Atom", "[test-3312][postfix]") {
    auto input = "arr[index]";

    auto expect = tommy_str(R"EOF(
       |-> PostfixSquareBracketsGroup
       |  -> Word: Atom: `arr`
       |  -> SquareBracketsGroup
       |    -> Term
       |      -> Word: Atom: `index`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeWord(input_iss);
    auto output_pw = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_pw);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("postfix sbg off of sbg", "[test-3313][postfix]") {
    auto input = "[elems][index]";

    auto expect = tommy_str(R"EOF(
       |-> PostfixSquareBracketsGroup
       |  -> Word: SquareBracketsGroup
       |    -> Term
       |      -> Word: Atom: `elems`
       |  -> SquareBracketsGroup
       |    -> Term
       |      -> Word: Atom: `index`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeSquareBracketsGroup(input_iss);
    auto output_word = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("postfix pg off of pg", "[test-3314][postfix]") {
    auto input = "(func)(arg)";

    auto expect = tommy_str(R"EOF(
       |-> PostfixParenthesesGroup
       |  -> Word: ParenthesesGroup
       |    -> Term
       |      -> Word: Atom: `func`
       |  -> ParenthesesGroup
       |    -> Term
       |      -> Word: Atom: `arg`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeParenthesesGroup(input_iss);
    auto output_word = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("postfix sbg off of pg", "[test-3315][postfix]") {
    auto input = "(arr)[index]";

    auto expect = tommy_str(R"EOF(
       |-> PostfixSquareBracketsGroup
       |  -> Word: ParenthesesGroup
       |    -> Term
       |      -> Word: Atom: `arr`
       |  -> SquareBracketsGroup
       |    -> Term
       |      -> Word: Atom: `index`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeParenthesesGroup(input_iss);
    auto output_word = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("postfix pg off of cbg", "[test-3316][postfix]") {
    auto input = "{func}(arg)";

    auto expect = tommy_str(R"EOF(
       |-> PostfixParenthesesGroup
       |  -> Word: CurlyBracketsGroup
       |    -> Term
       |      -> Word: Atom: `func`
       |  -> ParenthesesGroup
       |    -> Term
       |      -> Word: Atom: `arg`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(input_iss);
    auto output_word = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}

////////////////////////////////////////////////////////////////

TEST_CASE ("postfix sbg off of cbg", "[test-3317][postfix]") {
    auto input = "{arr}[index]";

    auto expect = tommy_str(R"EOF(
       |-> PostfixSquareBracketsGroup
       |  -> Word: CurlyBracketsGroup
       |    -> Term
       |      -> Word: Atom: `arr`
       |  -> SquareBracketsGroup
       |    -> Term
       |      -> Word: Atom: `index`
    )EOF");

    auto input_iss = std::istringstream(input);
    auto output = consumeCurlyBracketsGroup(input_iss);
    auto output_word = mayfail_cast<ProgramWord_>(output);
    auto output_str = montree::astToString(output_word);
    REQUIRE (output_str == expect);
}
