#include <catch2/catch_amalgamated.hpp>

#include <SquareBracketsGroup.h>
#include <ParenthesesGroup.h>
#include <utils/str-utils.h>

TEST_CASE("no SquareBracketsGroup expression found", "[test-1000][strictly]")
{
    auto iss = std::istringstream("fds");

    auto ret = tryConsumeSquareBracketsGroup(iss);

    REQUIRE (false == ret.has_value());
}

TEST_CASE("err-105 premature EOF after SquareBracketsGroup's initiator", "[test-1001][strictly]")
{
    auto iss = std::istringstream("[");

    auto ret = tryConsumeSquareBracketsGroup(iss);

    REQUIRE (true == ret.has_value());

    auto expect = tommy_str(R"END(
        -> SquareBracketsGroup: `[`
          -> ERR-105
    )END");
    REQUIRE (expect == montree::astToString(ret));
}

// TEST_CASE("err-114 invalid program word", "[test-1004][strictly]")
// {
//     std::istringstream iss(
//         "]"
//     );

//     auto ret = consumeProgramWord(iss);

//     REQUIRE (!ret);
//     REQUIRE (ret.error().code == 114);

//     REQUIRE (ret.error().cause);
//     REQUIRE (ret.error().cause.value()->code == 118);
// }

TEST_CASE("empty group", "[test-1002][strictly]")
{
    auto iss = std::istringstream("[]");

    auto ret = tryConsumeSquareBracketsGroup(iss);

    REQUIRE (true == ret.has_value());

    auto expect = tommy_str(R"END(
        -> SquareBracketsGroup: `[]`
    )END");
    REQUIRE (expect == montree::astToString(ret));
}

// TEST_CASE("err-109 was expecting SquareBracketsGroup's terminator", "[test-1005][strictly]")
// {
//     std::istringstream iss(
//         "[fds"
//     );
    
//     auto ret = tryConsumeSquareBracketsGroup(iss);
//     REQUIRE (ret);
//     REQUIRE (!*ret);
//     REQUIRE (ret->error().code == 109);
// }

// TEST_CASE("err-106 was expecting a valid Term in SquareBracketsGroup", "[test-1007][strictly]")
// {
//     std::istringstream iss(
//         "[fds)"
//     );

//     auto ret = tryConsumeSquareBracketsGroup(iss);
//     REQUIRE (ret);

//     REQUIRE (!*ret);
//     REQUIRE (ret->error().code == 106);

//     REQUIRE (ret->error().cause);
//     REQUIRE (ret->error().cause.value()->code == 116);

//     REQUIRE (ret->error().cause.value()->cause);
//     REQUIRE (ret->error().cause.value()->cause.value()->code == 101);
// }

// TEST_CASE("err-107 was expecting SquareBracketsGroup's continuator", "[test-1003][strictly]")
// {
//     std::istringstream iss(
//         "[fds,"
//     );

//     auto ret = tryConsumeSquareBracketsGroup(iss);
//     REQUIRE (ret);

//     REQUIRE (!*ret);
//     REQUIRE (ret->error().code == 107);

//     REQUIRE (ret->error().cause);
//     REQUIRE (ret->error().cause.value()->code == 101);
// }

// TEST_CASE("err-108 was expecting another ProgramWord after SquareBracketsGroup's continuator", "[test-1008][strictly]")
// {
//     std::istringstream iss(
//         "[fds, ]"
//     );

//     auto ret = tryConsumeSquareBracketsGroup(iss);
//     REQUIRE (ret);
//     REQUIRE (!*ret);
//     REQUIRE (ret->error().code == 108);
// }

// TEST_CASE("single atom", "[test-1009][strictly]")
// {
//     std::istringstream iss(
//         "[fds]"
//     );

//     auto ret = tryConsumeSquareBracketsGroup(iss);
//     REQUIRE (ret);
//     REQUIRE (*ret);

//     auto sbg = std::get<SquareBracketsGroup*>(**ret);
//     REQUIRE (sbg->terms.size() == 1);

//     auto term1 = sbg->terms[0];
//     REQUIRE (term1.words.size() == 1);

//     auto term1_word1 = std::get<Atom>(term1.words[0]);
//     REQUIRE (term1_word1.value == "fds");
// }

TEST_CASE("multiple terms, multiple atoms", "[test-1010][strictly]")
{
    auto iss = std::istringstream("[3, 1 + 2]");

    auto ret = tryConsumeSquareBracketsGroup(iss);

    REQUIRE (true == ret.has_value());

    auto expect = tommy_str(R"END(
        -> SquareBracketsGroup: `[3, 1 + 2]`
          -> term 1 -> one word: Atom: `3`
          -> term 2
            -> word 1: Atom: `1`
            -> word 2: Atom: `+`
            -> word 3: Atom: `2`
    )END");
    REQUIRE (expect == montree::astToString(ret));
}

// TEST_CASE("nested square brackets group", "[test-1011][strictly]")
// {
//     std::istringstream iss(
//         "[[fds]]"
//     );

//     auto ret = tryConsumeSquareBracketsGroup(iss);
//     REQUIRE (ret);
//     REQUIRE (*ret);

//     auto sbg = std::get<SquareBracketsGroup*>(**ret);
//     REQUIRE (sbg->terms.size() == 1);

//     auto term1 = sbg->terms[0];
//     REQUIRE (term1.words.size() == 1);

//     auto term1_word1 = std::get<SquareBracketsGroup*>(term1.words[0]);
//     REQUIRE (term1_word1->terms.size() == 1);

//     auto term1_word1_term1 = term1_word1->terms[0];
//     REQUIRE (term1_word1_term1.words.size() == 1);

//     auto term1_word1_term1_word1 = std::get<Atom>(term1_word1_term1.words[0]);
//     REQUIRE (term1_word1_term1_word1.value == "fds");

// }

TEST_CASE("parentheses group", "[test-1012][strictly]")
{
    auto iss = std::istringstream("[(a, b, c)]");

    auto ret = tryConsumeSquareBracketsGroup(iss);

    REQUIRE (true == ret.has_value());

    auto expect = tommy_str(R"END(
        -> SquareBracketsGroup: `[(a, b, c)]`
          -> one term -> one word: ParenthesesGroup: `(a, b, c)`
            -> term 1 -> one word: Atom: `a`
            -> term 2 -> one word: Atom: `b`
            -> term 3 -> one word: Atom: `c`
    )END");
    REQUIRE (expect == montree::astToString(ret));
}
