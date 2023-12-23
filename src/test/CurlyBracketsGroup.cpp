#include <CurlyBracketsGroup.h>
#include <ParenthesesGroup.h>
#include <SquareBracketsGroup.h>
#include <common.h>

#include <catch2/catch_amalgamated.hpp>

TEST_CASE("ERROR unexpected EOF while entering a curly brackets group")
{
    std::istringstream iss("{");
    REQUIRE_THROWS(tryConsumeCurlyBracketsGroup(iss));
}

TEST_CASE("ERROR expected a word")
{
    std::istringstream iss("}");
    REQUIRE_THROWS(consumeProgramWord(iss));
}

TEST_CASE("ERROR expected end of curly brackets group but hit EOF")
{
    std::istringstream iss("{fds");
    REQUIRE_THROWS(tryConsumeCurlyBracketsGroup(iss));
}

TEST_CASE("ERROR expected curly brackets terminator")
{
    std::istringstream iss("{fds)");
    REQUIRE_THROWS(tryConsumeCurlyBracketsGroup(iss));
}

TEST_CASE("ERROR expected a program sentence")
{
    std::istringstream iss("{" + std::string(1, NEWLINE));
    REQUIRE_THROWS(tryConsumeCurlyBracketsGroup(iss));
}

TEST_CASE("ERROR expected program sentence terminator")
{
    std::istringstream iss("{" + std::string(1, NEWLINE)
            + "fds");
    REQUIRE_THROWS(tryConsumeCurlyBracketsGroup(iss));
}

TEST_CASE("ERROR expected end of curly brackets")
{
    std::istringstream iss("{" + std::string(1, NEWLINE)
            + "fds" + std::string(1, NEWLINE));
    REQUIRE_THROWS(tryConsumeCurlyBracketsGroup(iss));
}

TEST_CASE("empty curly brackets group")
{
    std::istringstream iss("{}");
    auto res = std::get<CurlyBracketsGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    REQUIRE (res->sentences.size() == 0);
}

TEST_CASE("oneline term")
{
    std::istringstream iss("{fds}");
    auto res = std::get<CurlyBracketsGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    REQUIRE (res->sentences.size() == 1);
    REQUIRE (res->sentences[0].term.words.size() == 1);
    auto word1 = std::get<Atom>(res->sentences[0].term.words[0]);
    REQUIRE (word1.value == "fds");
}

TEST_CASE("one sentence")
{
    std::istringstream iss("{" + std::string(1, NEWLINE)
            + "fds" + std::string(1, NEWLINE)
            + "}");
    auto res = std::get<CurlyBracketsGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    REQUIRE (res->sentences.size() == 1);
    REQUIRE (res->sentences[0].term.words.size() == 1);
    auto word1 = std::get<Atom>(res->sentences[0].term.words[0]);
    REQUIRE (word1.value == "fds");
}

TEST_CASE("two sentences with terms")
{
    std::istringstream iss("{" + std::string(1, NEWLINE)
            + "fds" + std::string(1, NEWLINE)
            + "foo bar" + std::string(1, NEWLINE)
            + "}");
    auto res = std::get<CurlyBracketsGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    REQUIRE (res->sentences.size() == 2);
    REQUIRE (res->sentences[0].term.words.size() == 1);
    REQUIRE (res->sentences[1].term.words.size() == 2);
    auto sentence1_term1_word1 = std::get<Atom>(res->sentences[0].term.words[0]);
    auto sentence2_term1_word1 = std::get<Atom>(res->sentences[1].term.words[0]);
    auto sentence2_term1_word2 = std::get<Atom>(res->sentences[1].term.words[1]);
    REQUIRE (sentence1_term1_word1.value == "fds");
    REQUIRE (sentence2_term1_word1.value == "foo");
    REQUIRE (sentence2_term1_word2.value == "bar");
}

TEST_CASE("parentheses group and square brackets group")
{
    std::istringstream iss("{" + std::string(1, NEWLINE)
            + "(1 + 2, 3)" + std::string(1, NEWLINE)
            + "[fds, sdf]" + std::string(1, NEWLINE)
            + "}");
    auto res = std::get<CurlyBracketsGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    REQUIRE (res->sentences.size() == 2);
    REQUIRE (res->sentences[0].term.words.size() == 1);
    REQUIRE (res->sentences[1].term.words.size() == 1);

    auto sentence1_word1 = std::get<ParenthesesGroup*>(res->sentences[0].term.words[0]);
    auto sentence1_word1_term1_word1 = std::get<Atom>(sentence1_word1->terms[0].words[0]);
    auto sentence1_word1_term1_word2 = std::get<Atom>(sentence1_word1->terms[0].words[1]);
    auto sentence1_word1_term1_word3 = std::get<Atom>(sentence1_word1->terms[0].words[2]);
    auto sentence1_word1_term2_word1 = std::get<Atom>(sentence1_word1->terms[1].words[0]);
    REQUIRE (sentence1_word1->terms[0].words.size() == 3);
    REQUIRE (sentence1_word1->terms[1].words.size() == 1);
    REQUIRE (sentence1_word1_term1_word1.value == "1");
    REQUIRE (sentence1_word1_term1_word2.value == "+");
    REQUIRE (sentence1_word1_term1_word3.value == "2");
    REQUIRE (sentence1_word1_term2_word1.value == "3");

    auto sentence2_word1 = std::get<SquareBracketsGroup*>(res->sentences[1].term.words[0]);
    auto sentence2_word1_word1 = std::get<Atom>(sentence2_word1->terms[0].words[0]);
    auto sentence2_word1_word2 = std::get<Atom>(sentence2_word1->terms[1].words[0]);
    REQUIRE (sentence2_word1->terms[0].words.size() == 1);
    REQUIRE (sentence2_word1->terms[1].words.size() == 1);
    REQUIRE (sentence2_word1_word1.value == "fds");
    REQUIRE (sentence2_word1_word2.value == "sdf");
}

TEST_CASE("nested curly brackets group")
{
    std::istringstream iss("{" + std::string(1, NEWLINE)
            + "{" + std::string(1, NEWLINE)
            + "fds" + std::string(1, NEWLINE)
            + "}" + std::string(1, NEWLINE)
            + "}");
    auto res = std::get<CurlyBracketsGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    REQUIRE (res->sentences.size() == 1);
    REQUIRE (res->sentences[0].term.words.size() == 1);

    auto word1 = std::get<CurlyBracketsGroup*>(res->sentences[0].term.words[0]);
    REQUIRE (res->sentences.size() == 1);
    REQUIRE (res->sentences[0].term.words.size() == 1);
    auto word1_word1 = std::get<Atom>(word1->sentences[0].term.words[0]);
    REQUIRE (word1_word1.value == "fds");
}
