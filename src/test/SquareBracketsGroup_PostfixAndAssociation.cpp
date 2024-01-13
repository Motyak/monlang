#include <catch2/catch_amalgamated.hpp>

#include <SquareBracketsGroup.h>
#include <PostfixSquareBracketsGroup.h>
#include <PostfixParenthesesGroup.h>
#include <Association.h>

// TEST_CASE("postfix square brackets group", "[test-1200][postfix-and-assoc]")
// {
//     std::istringstream iss(
//         "[fds][sdf]"
//     );

//     auto ret = tryConsumeSquareBracketsGroup(iss);
//     REQUIRE (ret);
//     REQUIRE (*ret);

//     auto postfixSbg = std::get<PostfixSquareBracketsGroup*>(**ret);

//     auto leftPart = std::get<SquareBracketsGroup*>(postfixSbg->leftPart);
//     REQUIRE (leftPart->terms.size() == 1);
//     auto rightPart = postfixSbg->rightPart;
//     REQUIRE (rightPart->terms.size() == 1);

//     auto leftPart_term1 = leftPart->terms[0];
//     REQUIRE (leftPart_term1.words.size() == 1);
//     auto rightPart_term1 = rightPart->terms[0];
//     REQUIRE (rightPart_term1.words.size() == 1);

//     auto leftPart_term1_word1 = std::get<Atom>(leftPart_term1.words[0]);
//     REQUIRE (leftPart_term1_word1.value == "fds");
//     auto rightPart_term1_word1 = std::get<Atom>(rightPart_term1.words[0]);
//     REQUIRE (rightPart_term1_word1.value == "sdf");
// }

// TEST_CASE("chained postfix square brackets group", "[test-1201][postfix-and-assoc]")
// {
//     std::istringstream iss(
//         "[a][b][c]"
//     );

//     auto ret = tryConsumeSquareBracketsGroup(iss);
//     REQUIRE (ret);
//     REQUIRE (*ret);

//     auto postfixSbg = std::get<PostfixSquareBracketsGroup*>(**ret);

//     auto leftPart = std::get<PostfixSquareBracketsGroup*>(postfixSbg->leftPart);
//     auto rightPart = postfixSbg->rightPart;
//     REQUIRE (rightPart->terms.size() == 1);
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

// TEST_CASE("postfix parentheses group", "[test-1202][postfix-and-assoc]")
// {
//     std::istringstream iss(
//         "[fds](sdf)"
//     );

//     auto ret = tryConsumeSquareBracketsGroup(iss);
//     REQUIRE (ret);
//     REQUIRE (*ret);

//     auto postfixPg = std::get<PostfixParenthesesGroup*>(**ret);

//     auto leftPart = std::get<SquareBracketsGroup*>(postfixPg->leftPart);
//     REQUIRE (leftPart->terms.size() == 1);
//     auto rightPart = postfixPg->rightPart;
//     REQUIRE (rightPart->terms.size() == 1);

//     auto leftPart_term1 = leftPart->terms[0];
//     REQUIRE (leftPart_term1.words.size() == 1);
//     auto rightPart_term1 = rightPart->terms[0];
//     REQUIRE (rightPart_term1.words.size() == 1);

//     auto leftPart_term1_word1 = std::get<Atom>(leftPart_term1.words[0]);
//     REQUIRE (leftPart_term1_word1.value == "fds");
//     auto rightPart_term1_word1 = std::get<Atom>(rightPart_term1.words[0]);
//     REQUIRE (rightPart_term1_word1.value == "sdf");
// }

// TEST_CASE("chained postfix parentheses group", "[test-1203][postfix-and-assoc]")
// {
//     std::istringstream iss(
//         "[a](b)(c)"
//     );

//     auto ret = tryConsumeSquareBracketsGroup(iss);
//     REQUIRE (ret);
//     REQUIRE (*ret);

//     auto postfixPg = std::get<PostfixParenthesesGroup*>(**ret);

//     auto leftPart = std::get<PostfixParenthesesGroup*>(postfixPg->leftPart);
//     auto rightPart = postfixPg->rightPart;
//     REQUIRE (rightPart->terms.size() == 1);
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
//     std::istringstream iss(
//         "[fds]:sdf"
//     );

//     auto ret = tryConsumeSquareBracketsGroup(iss);
//     REQUIRE (ret);
//     REQUIRE (*ret);

//     auto assoc = std::get<Association*>(**ret);
//     auto leftPart = std::get<SquareBracketsGroup*>(assoc->leftPart);
//     REQUIRE (leftPart->terms.size() == 1);
//     auto rightPart = std::get<Atom>(assoc->rightPart);

//     REQUIRE (leftPart->terms[0].words.size() == 1);
//     auto leftPart_word1 = std::get<Atom>(leftPart->terms[0].words[0]);
//     REQUIRE (leftPart_word1.value == "fds");
// }

// TEST_CASE("chained association")
// {
//     std::istringstream iss(
//         "[a]:[b]:[c]"
//     );

//     auto ret = tryConsumeSquareBracketsGroup(iss);
//     REQUIRE (ret);
//     REQUIRE (*ret);

//     auto assoc = std::get<Association*>(**ret);

//     auto leftPart = std::get<SquareBracketsGroup*>(assoc->leftPart);
//     auto rightPart = std::get<Association*>(assoc->rightPart);
//     REQUIRE (leftPart->terms.size() == 1);
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
