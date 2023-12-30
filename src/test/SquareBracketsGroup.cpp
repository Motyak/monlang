#include "SquareBracketsGroup_Strictly.cpp"

// #include <SquareBracketsGroup.h>
// #include <ParenthesesGroup.h>
// #include <PostfixSquareBracketsGroup.h>
// #include <PostfixParenthesesGroup.h>
// #include <Association.h>

// #include <catch2/catch_amalgamated.hpp>

// #include <sstream>
// #include <variant>



// TEST_CASE("ERROR expected a word")
// {
//     std::istringstream iss("]");
//     REQUIRE_THROWS(consumeProgramWord(iss));
// }

// TEST_CASE("ERROR expected end of square brackets group but hit EOF")
// {
//     std::istringstream iss("[fds");
//     REQUIRE_THROWS(tryConsumeSquareBracketsGroup(iss));
// }

// TEST_CASE("ERROR expected square brackets terminator")
// {
//     std::istringstream iss("[fds)");
//     REQUIRE_THROWS(tryConsumeSquareBracketsGroup(iss));
// }

// TEST_CASE("ERROR expected square brackets group continuator sequence")
// {
//     std::istringstream iss("[fds,]");
//     REQUIRE_THROWS(tryConsumeSquareBracketsGroup(iss));
// }

// TEST_CASE("ERROR expected another word")
// {
//     std::istringstream iss("[fds, ]");
//     REQUIRE_THROWS(tryConsumeSquareBracketsGroup(iss));
// }

// TEST_CASE("empty square brackets group")
// {
//     std::istringstream iss("[]");
//     auto res = std::get<SquareBracketsGroup*>(*tryConsumeSquareBracketsGroup(iss));
//     REQUIRE (res->terms.size() == 0);
// }

// TEST_CASE("single atom")
// {
//     std::istringstream iss("[fds]");
//     auto res = std::get<SquareBracketsGroup*>(*tryConsumeSquareBracketsGroup(iss));
//     REQUIRE (res->terms.size() == 1);
//     REQUIRE (res->terms[0].words.size() == 1);
//     auto word1 = std::get<Atom>(res->terms[0].words[0]);
//     REQUIRE (word1.value == "fds");
// }

// TEST_CASE("multiple atoms")
// {
//     std::istringstream iss("[fds, sdf]");
//     auto res = std::get<SquareBracketsGroup*>(*tryConsumeSquareBracketsGroup(iss));
//     REQUIRE (res->terms.size() == 2);
//     REQUIRE (res->terms[0].words.size() == 1);
//     REQUIRE (res->terms[1].words.size() == 1);
//     auto word1 = std::get<Atom>(res->terms[0].words[0]);
//     auto word2 = std::get<Atom>(res->terms[1].words[0]);
//     REQUIRE (word1.value == "fds");
//     REQUIRE (word2.value == "sdf");
// }

// TEST_CASE("terms")
// {
//     std::istringstream iss("[1 + 2, 3]");
//     auto res = std::get<SquareBracketsGroup*>(*tryConsumeSquareBracketsGroup(iss));
//     REQUIRE (res->terms.size() == 2);
//     REQUIRE (res->terms[0].words.size() == 3);
//     REQUIRE (res->terms[1].words.size() == 1);
//     auto term1_word1 = std::get<Atom>(res->terms[0].words[0]);
//     auto term1_word2 = std::get<Atom>(res->terms[0].words[1]);
//     auto term1_word3 = std::get<Atom>(res->terms[0].words[2]);
//     auto term2_word1 = std::get<Atom>(res->terms[1].words[0]);
//     REQUIRE (term1_word1.value == "1");
//     REQUIRE (term1_word2.value == "+");
//     REQUIRE (term1_word3.value == "2");
//     REQUIRE (term2_word1.value == "3");
// }

// TEST_CASE("nested square brackets group")
// {
//     std::istringstream iss("[[fds]]");
//     auto res = std::get<SquareBracketsGroup*>(*tryConsumeSquareBracketsGroup(iss));
//     REQUIRE (res->terms.size() == 1);
//     REQUIRE (res->terms[0].words.size() == 1);
//     auto word1 = std::get<SquareBracketsGroup*>(res->terms[0].words[0]);
//     REQUIRE (word1->terms.size() == 1);
//     REQUIRE (word1->terms[0].words.size() == 1);
//     auto word1_word1 = std::get<Atom>(word1->terms[0].words[0]);
//     REQUIRE (word1_word1.value == "fds");
// }

// TEST_CASE("parentheses group")
// {
//     std::istringstream iss("[(a, b, c)]");
//     auto res = std::get<SquareBracketsGroup*>(*tryConsumeSquareBracketsGroup(iss));
//     REQUIRE (res->terms.size() == 1);
//     REQUIRE (res->terms[0].words.size() == 1);
//     auto word1 = std::get<ParenthesesGroup*>(res->terms[0].words[0]);
//     REQUIRE (word1->terms.size() == 3);
//     REQUIRE (word1->terms[0].words.size() == 1);
//     REQUIRE (word1->terms[1].words.size() == 1);
//     REQUIRE (word1->terms[2].words.size() == 1);
//     auto word1_word1 = std::get<Atom>(word1->terms[0].words[0]);
//     auto word1_word2 = std::get<Atom>(word1->terms[1].words[0]);
//     auto word1_word3 = std::get<Atom>(word1->terms[2].words[0]);
//     REQUIRE (word1_word1.value == "a");
//     REQUIRE (word1_word2.value == "b");
//     REQUIRE (word1_word3.value == "c");
// }

// TEST_CASE("postfix square brackets group")
// {
//     std::istringstream iss("[fds][sdf]");
//     auto res = std::get<PostfixSquareBracketsGroup*>(*tryConsumeSquareBracketsGroup(iss));
//     auto leftPart = std::get<SquareBracketsGroup*>(res->leftPart);
//     auto rightPart = res->rightPart;

//     REQUIRE (leftPart->terms.size() == 1);
//     REQUIRE (leftPart->terms[0].words.size() == 1);
//     auto leftPart_word1 = std::get<Atom>(leftPart->terms[0].words[0]);
//     REQUIRE (leftPart_word1.value == "fds");

//     REQUIRE (rightPart->terms.size() == 1);
//     REQUIRE (rightPart->terms[0].words.size() == 1);
//     auto rightPart_word1 = std::get<Atom>(rightPart->terms[0].words[0]);
//     REQUIRE (rightPart_word1.value == "sdf");
// }

// TEST_CASE("chained postfix square brackets group")
// {
//     std::istringstream iss("[a][b][c]");
//     auto res = std::get<PostfixSquareBracketsGroup*>(*tryConsumeSquareBracketsGroup(iss));
//     auto leftPart = std::get<PostfixSquareBracketsGroup*>(res->leftPart);
//     auto rightPart = res->rightPart;
//     auto leftPart_leftPart = std::get<SquareBracketsGroup*>(leftPart->leftPart);
//     auto leftPart_rightPart = leftPart->rightPart;

//     /* [a][b]>[c]< */
//     REQUIRE (rightPart->terms.size() == 1);
//     REQUIRE (rightPart->terms[0].words.size() == 1);
//     auto rightPart_atom = std::get<Atom>(rightPart->terms[0].words[0]);
//     REQUIRE (rightPart_atom.value == "c");

//     /* [a]>[b]<[c] */
//     REQUIRE (leftPart_rightPart->terms.size() == 1);
//     REQUIRE (leftPart_rightPart->terms[0].words.size() == 1);
//     auto leftPart_rightPart_atom = std::get<Atom>(leftPart_rightPart->terms[0].words[0]);
//     REQUIRE (leftPart_rightPart_atom.value == "b");

//     /* >[a]<[b][c] */
//     REQUIRE (leftPart_leftPart->terms.size() == 1);
//     REQUIRE (leftPart_leftPart->terms[0].words.size() == 1);
//     auto leftPart_leftPart_atom = std::get<Atom>(leftPart_leftPart->terms[0].words[0]);
//     REQUIRE (leftPart_leftPart_atom.value == "a");
// }

// TEST_CASE("postfix parentheses group")
// {
//     std::istringstream iss("[fds](sdf)");
//     auto res = std::get<PostfixParenthesesGroup*>(*tryConsumeSquareBracketsGroup(iss));
//     auto leftPart = std::get<SquareBracketsGroup*>(res->leftPart);
//     auto rightPart = res->rightPart;

//     REQUIRE (leftPart->terms.size() == 1);
//     REQUIRE (leftPart->terms[0].words.size() == 1);
//     auto leftPart_word1 = std::get<Atom>(leftPart->terms[0].words[0]);
//     REQUIRE (leftPart_word1.value == "fds");

//     REQUIRE (rightPart->terms.size() == 1);
//     REQUIRE (rightPart->terms[0].words.size() == 1);
//     auto rightPart_word1 = std::get<Atom>(rightPart->terms[0].words[0]);
//     REQUIRE (rightPart_word1.value == "sdf");
// }

// TEST_CASE("chained postfix parentheses group")
// {
//     std::istringstream iss("[a](b)(c)");
//     auto res = std::get<PostfixParenthesesGroup*>(*tryConsumeSquareBracketsGroup(iss));
//     auto leftPart = std::get<PostfixParenthesesGroup*>(res->leftPart);
//     auto rightPart = res->rightPart;
//     auto leftPart_leftPart = std::get<SquareBracketsGroup*>(leftPart->leftPart);
//     auto leftPart_rightPart = leftPart->rightPart;

//     /* [a](b)>(c)< */
//     REQUIRE (rightPart->terms.size() == 1);
//     REQUIRE (rightPart->terms[0].words.size() == 1);
//     auto rightPart_atom = std::get<Atom>(rightPart->terms[0].words[0]);
//     REQUIRE (rightPart_atom.value == "c");

//     /* [a]>(b)<(c) */
//     REQUIRE (leftPart_rightPart->terms.size() == 1);
//     REQUIRE (leftPart_rightPart->terms[0].words.size() == 1);
//     auto leftPart_rightPart_atom = std::get<Atom>(leftPart_rightPart->terms[0].words[0]);
//     REQUIRE (leftPart_rightPart_atom.value == "b");

//     /* >[a]<(b)(c) */
//     REQUIRE (leftPart_leftPart->terms.size() == 1);
//     REQUIRE (leftPart_leftPart->terms[0].words.size() == 1);
//     auto leftPart_leftPart_atom = std::get<Atom>(leftPart_leftPart->terms[0].words[0]);
//     REQUIRE (leftPart_leftPart_atom.value == "a");
// }

// TEST_CASE("association")
// {
//     std::istringstream iss("[fds]:sdf");
//     auto res = std::get<Association*>(*tryConsumeSquareBracketsGroup(iss));
//     auto leftPart = std::get<SquareBracketsGroup*>(res->leftPart);
//     auto rightPart = std::get<Atom>(res->rightPart);

//     REQUIRE (leftPart->terms.size() == 1);
//     REQUIRE (leftPart->terms[0].words.size() == 1);
//     auto leftPart_word1 = std::get<Atom>(leftPart->terms[0].words[0]);
//     REQUIRE (leftPart_word1.value == "fds");

//     REQUIRE (rightPart.value == "sdf");
// }

// TEST_CASE("chained association")
// {
//     std::istringstream iss("[a]:[b]:[c]");
//     auto res = std::get<Association*>(*tryConsumeSquareBracketsGroup(iss));
//     auto leftPart = std::get<SquareBracketsGroup*>(res->leftPart);
//     auto rightPart = std::get<Association*>(res->rightPart);
//     auto rightPart_leftPart = std::get<SquareBracketsGroup*>(rightPart->leftPart);
//     auto rightPart_rightPart = std::get<SquareBracketsGroup*>(rightPart->rightPart);

//     /* >[a]<:[b]:[c] */
//     REQUIRE (leftPart->terms.size() == 1);
//     REQUIRE (leftPart->terms[0].words.size() == 1);
//     auto leftPart_word1 = std::get<Atom>(leftPart->terms[0].words[0]);
//     REQUIRE (leftPart_word1.value == "a");

//     /* [a]:>[b]<:[c] */
//     REQUIRE (rightPart_leftPart->terms.size() == 1);
//     REQUIRE (rightPart_leftPart->terms[0].words.size() == 1);
//     auto rightPart_leftPart_word1 = std::get<Atom>(rightPart_leftPart->terms[0].words[0]);
//     REQUIRE (rightPart_leftPart_word1.value == "b");

//     /* [a]:[b]:>[c]< */
//     REQUIRE (rightPart_rightPart->terms.size() == 1);
//     REQUIRE (rightPart_rightPart->terms[0].words.size() == 1);
//     auto rightPart_rightPart_word1 = std::get<Atom>(rightPart_rightPart->terms[0].words[0]);
//     REQUIRE (rightPart_rightPart_word1.value == "c");
// }
