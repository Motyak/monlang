#include <catch2/catch_amalgamated.hpp>

#include <SquareBracketsGroup.h>
#include <ParenthesesGroup.h>

TEST_CASE("no SquareBracketsGroup expression found", "[test-1000][strictly]")
{
    std::istringstream iss(
        "fds"
    );

    auto ret = tryConsumeSquareBracketsGroup(iss);

    REQUIRE (!ret);
}

TEST_CASE("err-105 premature EOF after SquareBracketsGroup's initiator", "[test-1001][strictly]")
{
    std::istringstream iss(
        "["
    );

    auto ret = tryConsumeSquareBracketsGroup(iss);

    REQUIRE (ret);
    REQUIRE (!*ret);
    REQUIRE (ret->error().code == 105);
}

TEST_CASE("err-114 invalid program word", "[test-1004][strictly]")
{
    std::istringstream iss(
        "]"
    );

    auto ret = consumeProgramWord(iss);

    REQUIRE (!ret);
    REQUIRE (ret.error().code == 114);

    REQUIRE (ret.error().cause);
    REQUIRE (ret.error().cause.value()->code == 118);
}

TEST_CASE("empty group", "[test-1002][strictly]")
{
    std::istringstream iss(
        "[]"
    );

    auto ret = tryConsumeSquareBracketsGroup(iss);
    REQUIRE (ret);
    REQUIRE (*ret);
    auto sbg = std::get<SquareBracketsGroup*>(**ret);
    REQUIRE (sbg->terms.size() == 0);
}

TEST_CASE("err-109 was expecting SquareBracketsGroup's terminator", "[test-1005][strictly]")
{
    std::istringstream iss(
        "[fds"
    );
    
    auto ret = tryConsumeSquareBracketsGroup(iss);
    REQUIRE (ret);
    REQUIRE (!*ret);
    REQUIRE (ret->error().code == 109);
}

TEST_CASE("err-106 was expecting a valid Term in SquareBracketsGroup", "[test-1007][strictly]")
{
    std::istringstream iss(
        "[fds)"
    );

    auto ret = tryConsumeSquareBracketsGroup(iss);
    REQUIRE (ret);

    REQUIRE (!*ret);
    REQUIRE (ret->error().code == 106);

    REQUIRE (ret->error().cause);
    REQUIRE (ret->error().cause.value()->code == 116);

    REQUIRE (ret->error().cause.value()->cause);
    REQUIRE (ret->error().cause.value()->cause.value()->code == 101);
}

TEST_CASE("err-107 was expecting SquareBracketsGroup's continuator", "[test-1003][strictly]")
{
    std::istringstream iss(
        "[fds,"
    );

    auto ret = tryConsumeSquareBracketsGroup(iss);
    REQUIRE (ret);

    REQUIRE (!*ret);
    REQUIRE (ret->error().code == 107);

    REQUIRE (ret->error().cause);
    REQUIRE (ret->error().cause.value()->code == 101);
}

TEST_CASE("err-108 was expecting another ProgramWord after SquareBracketsGroup's continuator", "[test-1008][strictly]")
{
    std::istringstream iss(
        "[fds, ]"
    );

    auto ret = tryConsumeSquareBracketsGroup(iss);
    REQUIRE (ret);
    REQUIRE (!*ret);
    REQUIRE (ret->error().code == 108);
}

TEST_CASE("single atom", "[test-1009][strictly]")
{
    std::istringstream iss(
        "[fds]"
    );

    auto ret = tryConsumeSquareBracketsGroup(iss);
    REQUIRE (ret);
    REQUIRE (*ret);

    auto sbg = std::get<SquareBracketsGroup*>(**ret);
    REQUIRE (sbg->terms.size() == 1);

    auto term1 = sbg->terms[0];
    REQUIRE (term1.words.size() == 1);

    auto term1_word1 = std::get<Atom>(term1.words[0]);
    REQUIRE (term1_word1.value == "fds");
}

TEST_CASE("multiple terms, multiple atoms", "[test-1010][strictly]")
{
    std::istringstream iss(
        "[3, 1 + 2]"
    );
    
    auto ret = tryConsumeSquareBracketsGroup(iss);
    REQUIRE (ret);
    REQUIRE (*ret);

    auto sbg = std::get<SquareBracketsGroup*>(**ret);
    REQUIRE (sbg->terms.size() == 2);

    auto term1 = sbg->terms[0];
    REQUIRE (term1.words.size() == 1);
    auto term2 = sbg->terms[1];
    REQUIRE (term2.words.size() == 3);

    auto term1_word1 = std::get<Atom>(term1.words[0]);
    REQUIRE (term1_word1.value == "3");
    auto term2_word1 = std::get<Atom>(term2.words[0]);
    REQUIRE (term2_word1.value == "1");
    auto term2_word2 = std::get<Atom>(term2.words[1]);
    REQUIRE (term2_word2.value == "+");
    auto term2_word3 = std::get<Atom>(term2.words[2]);
    REQUIRE (term2_word3.value == "2");
}

TEST_CASE("nested square brackets group", "[test-1011][strictly]")
{
    std::istringstream iss(
        "[[fds]]"
    );

    auto ret = tryConsumeSquareBracketsGroup(iss);
    REQUIRE (ret);
    REQUIRE (*ret);

    auto sbg = std::get<SquareBracketsGroup*>(**ret);
    REQUIRE (sbg->terms.size() == 1);

    auto term1 = sbg->terms[0];
    REQUIRE (term1.words.size() == 1);

    auto term1_word1 = std::get<SquareBracketsGroup*>(term1.words[0]);
    REQUIRE (term1_word1->terms.size() == 1);

    auto term1_word1_term1 = term1_word1->terms[0];
    REQUIRE (term1_word1_term1.words.size() == 1);

    auto term1_word1_term1_word1 = std::get<Atom>(term1_word1_term1.words[0]);
    REQUIRE (term1_word1_term1_word1.value == "fds");

}

// TEST_CASE("parentheses group", "[test-1012][strictly]")
// {
//     std::istringstream iss(
//         "[(a, b, c)]"
//     );

//     auto ret = tryConsumeSquareBracketsGroup(iss);
//     REQUIRE (ret);
//     REQUIRE (*ret);

//     auto sbg = std::get<SquareBracketsGroup*>(**ret);
//     REQUIRE (sbg->terms.size() == 1);

//     auto term1 = sbg->terms[0];
//     REQUIRE (term1.words.size() == 1);

//     auto term1_word1 = std::get<ParenthesesGroup*>(term1.words[0]);
//     REQUIRE (term1_word1->terms.size() == 3);

//     auto term1_word1_term1 = term1_word1->terms[0];
//     REQUIRE (term1_word1_term1.words.size() == 1);
//     auto term1_word1_term2 = term1_word1->terms[1];
//     REQUIRE (term1_word1_term2.words.size() == 1);
//     auto term1_word1_term3 = term1_word1->terms[2];
//     REQUIRE (term1_word1_term3.words.size() == 1);

//     auto term1_word1_term1_word1 = std::get<Atom>(term1_word1_term1.words[0]);
//     REQUIRE (term1_word1_term1_word1.value == "a");
//     auto term1_word1_term2_word1 = std::get<Atom>(term1_word1_term2.words[0]);
//     REQUIRE (term1_word1_term2_word1.value == "b");
//     auto term1_word1_term3_word1 = std::get<Atom>(term1_word1_term3.words[0]);
//     REQUIRE (term1_word1_term3_word1.value == "c");
// }
