#include <SquareBracketsGroup.h>
#include <Association.h>

#include <catch2/catch_amalgamated.hpp>

#include <sstream>
#include <variant>

TEST_CASE("ERROR unexpected EOF while entering a square brackets group")
{
    std::istringstream iss("[");
    REQUIRE_THROWS(tryConsumeSquareBracketsGroup(iss));
}

TEST_CASE("empty square brackets group")
{
    std::istringstream iss("[]");
    auto res = std::get<SquareBracketsGroup*>(*tryConsumeSquareBracketsGroup(iss));
    REQUIRE (res->terms.size() == 0);
}

TEST_CASE("single atom")
{
    std::istringstream iss("[fds]");
    auto res = std::get<SquareBracketsGroup*>(*tryConsumeSquareBracketsGroup(iss));
    REQUIRE (res->terms.size() == 1);
    REQUIRE (res->terms[0].words.size() == 1);
    auto word1 = std::get<Atom>(res->terms[0].words[0]);
    REQUIRE (word1.value == "fds");
}

TEST_CASE("multiple atoms")
{
    std::istringstream iss("[fds, sdf]");
    auto res = std::get<SquareBracketsGroup*>(*tryConsumeSquareBracketsGroup(iss));
    REQUIRE (res->terms.size() == 2);
    REQUIRE (res->terms[0].words.size() == 1);
    REQUIRE (res->terms[1].words.size() == 1);
    auto word1 = std::get<Atom>(res->terms[0].words[0]);
    auto word2 = std::get<Atom>(res->terms[1].words[0]);
    REQUIRE (word1.value == "fds");
    REQUIRE (word2.value == "sdf");
}

TEST_CASE("nested square brackets group")
{
    std::istringstream iss("[[fds]]");
    auto res = std::get<SquareBracketsGroup*>(*tryConsumeSquareBracketsGroup(iss));
    REQUIRE (res->terms.size() == 1);
    REQUIRE (res->terms[0].words.size() == 1);
    auto word1 = std::get<SquareBracketsGroup*>(res->terms[0].words[0]);
    REQUIRE (word1->terms.size() == 1);
    REQUIRE (word1->terms[0].words.size() == 1);
    auto word1_word1 = std::get<Atom>(word1->terms[0].words[0]);
    REQUIRE (word1_word1.value == "fds");
}

TEST_CASE("association")
{
    std::istringstream iss("[fds]:sdf");
    auto res = std::get<Association*>(*tryConsumeSquareBracketsGroup(iss));
    auto leftPart = std::get<SquareBracketsGroup*>(res->leftPart);
    auto rightPart = std::get<Atom>(res->rightPart);

    REQUIRE (leftPart->terms.size() == 1);
    REQUIRE (leftPart->terms[0].words.size() == 1);
    auto leftPart_word1 = std::get<Atom>(leftPart->terms[0].words[0]);
    REQUIRE (leftPart_word1.value == "fds");

    REQUIRE (rightPart.value == "sdf");
}
