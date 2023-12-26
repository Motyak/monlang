#include <CurlyBracketsGroup.h>
#include <ParenthesesGroup.h>
#include <SquareBracketsGroup.h>
#include <PostfixParenthesesGroup.h>
#include <PostfixSquareBracketsGroup.h>
#include <Association.h>

#include <catch2/catch_amalgamated.hpp>

const auto LF = std::string(1, NEWLINE);

TEST_CASE("ERROR expected a word", "[multiline]")
{
    std::istringstream iss("{" + LF);
    REQUIRE_THROWS(consumeProgramWord(iss));
}

TEST_CASE("ERROR expected program sentence terminator but hit EOF", "[multiline]")
{
    std::istringstream iss("{" + LF
            + TAB + "fds");
    REQUIRE_THROWS(tryConsumeCurlyBracketsGroup(iss));
}

TEST_CASE("ERROR expected end of curly brackets", "[multiline]")
{
    std::istringstream iss("{" + LF
            + TAB + "fds" + LF);
    REQUIRE_THROWS(tryConsumeCurlyBracketsGroup(iss));
}

TEST_CASE("one sentence", "[multiline]")
{
    std::istringstream iss("{" + LF
            + TAB + "fds" + LF
            + "}");
    auto res = std::get<CurlyBracketsGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    REQUIRE (res->sentences.size() == 1);
    REQUIRE (res->sentences[0].words.size() == 1);
    auto word1 = std::get<Atom>(res->sentences[0].words[0]);
    REQUIRE (word1.value == "fds");
}

TEST_CASE("two sentences with terms", "[multiline]")
{
    std::istringstream iss("{" + LF
            + TAB + "fds" + LF
            + TAB + "foo bar" + LF
            + "}");
    auto res = std::get<CurlyBracketsGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    REQUIRE (res->sentences.size() == 2);
    REQUIRE (res->sentences[0].words.size() == 1);
    REQUIRE (res->sentences[1].words.size() == 2);
    auto sentence1_term1_word1 = std::get<Atom>(res->sentences[0].words[0]);
    auto sentence2_term1_word1 = std::get<Atom>(res->sentences[1].words[0]);
    auto sentence2_term1_word2 = std::get<Atom>(res->sentences[1].words[1]);
    REQUIRE (sentence1_term1_word1.value == "fds");
    REQUIRE (sentence2_term1_word1.value == "foo");
    REQUIRE (sentence2_term1_word2.value == "bar");
}

TEST_CASE("parentheses group and square brackets group", "[multiline]")
{
    std::istringstream iss("{" + LF
            + TAB + "(1 + 2, 3)" + LF
            + TAB + "[fds, sdf]" + LF
            + "}");
    auto res = std::get<CurlyBracketsGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    REQUIRE (res->sentences.size() == 2);
    REQUIRE (res->sentences[0].words.size() == 1);
    REQUIRE (res->sentences[1].words.size() == 1);

    auto sentence1_word1 = std::get<ParenthesesGroup*>(res->sentences[0].words[0]);
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

    auto sentence2_word1 = std::get<SquareBracketsGroup*>(res->sentences[1].words[0]);
    auto sentence2_word1_word1 = std::get<Atom>(sentence2_word1->terms[0].words[0]);
    auto sentence2_word1_word2 = std::get<Atom>(sentence2_word1->terms[1].words[0]);
    REQUIRE (sentence2_word1->terms[0].words.size() == 1);
    REQUIRE (sentence2_word1->terms[1].words.size() == 1);
    REQUIRE (sentence2_word1_word1.value == "fds");
    REQUIRE (sentence2_word1_word2.value == "sdf");
}

TEST_CASE("nested curly brackets group", "[multiline]")
{
    std::istringstream iss("{" + LF
            + TAB + "{" + LF
            + TAB + TAB + "fds" + LF
            + TAB + "}" + LF
            + "}");
    auto res = std::get<CurlyBracketsGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    REQUIRE (res->sentences.size() == 1);
    REQUIRE (res->sentences[0].words.size() == 1);

    auto word1 = std::get<CurlyBracketsGroup*>(res->sentences[0].words[0]);
    REQUIRE (res->sentences.size() == 1);
    REQUIRE (res->sentences[0].words.size() == 1);
    auto word1_word1 = std::get<Atom>(word1->sentences[0].words[0]);
    REQUIRE (word1_word1.value == "fds");
}
