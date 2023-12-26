#include <CurlyBracketsGroup.h>
#include <ParenthesesGroup.h>
#include <SquareBracketsGroup.h>
#include <PostfixParenthesesGroup.h>
#include <PostfixSquareBracketsGroup.h>
#include <Association.h>

#include <catch2/catch_amalgamated.hpp>

TEST_CASE("ERROR unexpected EOF while entering a oneline curly brackets group", "[oneline]")
{
    std::istringstream iss("{");
    REQUIRE_THROWS(tryConsumeCurlyBracketsGroup(iss));
}

TEST_CASE("ERROR expected a word", "[oneline]")
{
    std::istringstream iss("}");
    REQUIRE_THROWS(consumeProgramWord(iss));
}

TEST_CASE("ERROR expected end of curly brackets group but hit EOF", "[oneline]")
{
    std::istringstream iss("{fds");
    REQUIRE_THROWS(tryConsumeCurlyBracketsGroup(iss));
}

TEST_CASE("ERROR expected curly brackets terminator", "[oneline]")
{
    std::istringstream iss("{fds)");
    REQUIRE_THROWS(tryConsumeCurlyBracketsGroup(iss));
}

TEST_CASE("empty curly brackets group", "[oneline]")
{
    std::istringstream iss("{}");
    auto res = std::get<CurlyBracketsGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    REQUIRE (res->sentences.size() == 0);
}

TEST_CASE("one sentence two words", "[oneline]")
{
    std::istringstream iss("{fds sdf}");
    auto res = std::get<CurlyBracketsGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    REQUIRE (res->sentences.size() == 1);
    REQUIRE (res->sentences[0].words.size() == 2);
    auto word1 = std::get<Atom>(res->sentences[0].words[0]);
    auto word2 = std::get<Atom>(res->sentences[0].words[1]);
    REQUIRE (word1.value == "fds");
    REQUIRE (word2.value == "sdf");
}

TEST_CASE("postfix parentheses group", "[oneline]")
{
    std::istringstream iss("{fds}(sdf)");
    auto res = std::get<PostfixParenthesesGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    auto leftPart = std::get<CurlyBracketsGroup*>(res->leftPart);
    auto rightPart = res->rightPart;

    REQUIRE (leftPart->sentences.size() == 1);
    REQUIRE (leftPart->sentences[0].words.size() == 1);
    auto leftPart_sentence1_word1 = std::get<Atom>(leftPart->sentences[0].words[0]);
    REQUIRE (leftPart_sentence1_word1.value == "fds");

    REQUIRE (rightPart->terms.size() == 1);
    REQUIRE (rightPart->terms[0].words.size() == 1);
    auto rightPart_word1 = std::get<Atom>(rightPart->terms[0].words[0]);
    REQUIRE (rightPart_word1.value == "sdf");
}

TEST_CASE("chained postfix parentheses group", "[oneline]")
{
    std::istringstream iss("{a}(b)(c)");
    auto res = std::get<PostfixParenthesesGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    auto leftPart = std::get<PostfixParenthesesGroup*>(res->leftPart);
    auto rightPart = res->rightPart;
    auto leftPart_leftPart = std::get<CurlyBracketsGroup*>(leftPart->leftPart);
    auto leftPart_rightPart = leftPart->rightPart;

    /* {a}(b)>(c)< */
    REQUIRE (rightPart->terms.size() == 1);
    REQUIRE (rightPart->terms[0].words.size() == 1);
    auto rightPart_atom = std::get<Atom>(rightPart->terms[0].words[0]);
    REQUIRE (rightPart_atom.value == "c");

    /* {a}>(b)<(c) */
    REQUIRE (leftPart_rightPart->terms.size() == 1);
    REQUIRE (leftPart_rightPart->terms[0].words.size() == 1);
    auto leftPart_rightPart_atom = std::get<Atom>(leftPart_rightPart->terms[0].words[0]);
    REQUIRE (leftPart_rightPart_atom.value == "b");

    /* >{a}<(b)(c) */
    REQUIRE (leftPart_leftPart->sentences.size() == 1);
    REQUIRE (leftPart_leftPart->sentences[0].words.size() == 1);
    auto leftPart_leftPart_atom = std::get<Atom>(leftPart_leftPart->sentences[0].words[0]);
    REQUIRE (leftPart_leftPart_atom.value == "a");
}

TEST_CASE("postfix square brackets group", "[oneline]")
{
    std::istringstream iss("{fds}[sdf]");
    auto res = std::get<PostfixSquareBracketsGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    auto leftPart = std::get<CurlyBracketsGroup*>(res->leftPart);
    auto rightPart = res->rightPart;

    REQUIRE (leftPart->sentences.size() == 1);
    REQUIRE (leftPart->sentences[0].words.size() == 1);
    auto leftPart_sentence1_word1 = std::get<Atom>(leftPart->sentences[0].words[0]);
    REQUIRE (leftPart_sentence1_word1.value == "fds");

    REQUIRE (rightPart->terms.size() == 1);
    REQUIRE (rightPart->terms[0].words.size() == 1);
    auto rightPart_word1 = std::get<Atom>(rightPart->terms[0].words[0]);
    REQUIRE (rightPart_word1.value == "sdf");
}

TEST_CASE("chained postfix square brackets group", "[oneline]")
{
    std::istringstream iss("{a}[b][c]");
    auto res = std::get<PostfixSquareBracketsGroup*>(*tryConsumeCurlyBracketsGroup(iss));
    auto leftPart = std::get<PostfixSquareBracketsGroup*>(res->leftPart);
    auto rightPart = res->rightPart;
    auto leftPart_leftPart = std::get<CurlyBracketsGroup*>(leftPart->leftPart);
    auto leftPart_rightPart = leftPart->rightPart;

    /* {a}[b]>[c]< */
    REQUIRE (rightPart->terms.size() == 1);
    REQUIRE (rightPart->terms[0].words.size() == 1);
    auto rightPart_atom = std::get<Atom>(rightPart->terms[0].words[0]);
    REQUIRE (rightPart_atom.value == "c");

    /* {a}>[b]<[c] */
    REQUIRE (leftPart_rightPart->terms.size() == 1);
    REQUIRE (leftPart_rightPart->terms[0].words.size() == 1);
    auto leftPart_rightPart_atom = std::get<Atom>(leftPart_rightPart->terms[0].words[0]);
    REQUIRE (leftPart_rightPart_atom.value == "b");

    /* >{a}<[b][c] */
    REQUIRE (leftPart_leftPart->sentences.size() == 1);
    REQUIRE (leftPart_leftPart->sentences[0].words.size() == 1);
    auto leftPart_leftPart_atom = std::get<Atom>(leftPart_leftPart->sentences[0].words[0]);
    REQUIRE (leftPart_leftPart_atom.value == "a");
}

TEST_CASE("association", "[oneline]")
{
    std::istringstream iss("{fds}:sdf");
    auto res = std::get<Association*>(*tryConsumeCurlyBracketsGroup(iss));
    auto leftPart = std::get<CurlyBracketsGroup*>(res->leftPart);
    auto rightPart = std::get<Atom>(res->rightPart);

    REQUIRE (leftPart->sentences.size() == 1);
    REQUIRE (leftPart->sentences[0].words.size() == 1);
    auto leftPart_word1 = std::get<Atom>(leftPart->sentences[0].words[0]);
    REQUIRE (leftPart_word1.value == "fds");

    REQUIRE (rightPart.value == "sdf");
}

TEST_CASE("chained association", "[oneline]")
{
    std::istringstream iss("{a}:{b}:{c}");
    auto res = std::get<Association*>(*tryConsumeCurlyBracketsGroup(iss));
    auto leftPart = std::get<CurlyBracketsGroup*>(res->leftPart);
    auto rightPart = std::get<Association*>(res->rightPart);
    auto rightPart_leftPart = std::get<CurlyBracketsGroup*>(rightPart->leftPart);
    auto rightPart_rightPart = std::get<CurlyBracketsGroup*>(rightPart->rightPart);

    /* {a}:{b}:>{c}< */
    REQUIRE (leftPart->sentences.size() == 1);
    REQUIRE (leftPart->sentences[0].words.size() == 1);
    auto leftPart_word1 = std::get<Atom>(leftPart->sentences[0].words[0]);
    REQUIRE (leftPart_word1.value == "a");

    /* {a}:>{b}<:{c} */
    REQUIRE (rightPart_leftPart->sentences.size() == 1);
    REQUIRE (rightPart_leftPart->sentences[0].words.size() == 1);
    auto rightPart_leftPart_word1 = std::get<Atom>(rightPart_leftPart->sentences[0].words[0]);
    REQUIRE (rightPart_leftPart_word1.value == "b");

    /* >{a}<:{b}:{c} */
    REQUIRE (rightPart_rightPart->sentences.size() == 1);
    REQUIRE (rightPart_rightPart->sentences[0].words.size() == 1);
    auto rightPart_rightPart_word1 = std::get<Atom>(rightPart_rightPart->sentences[0].words[0]);
    REQUIRE (rightPart_rightPart_word1.value == "c");
}
