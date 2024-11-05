#ifndef WORD_H
#define WORD_H

#include <monlang/common.h>

#include <utils/variant-utils.h>

#include <sstream>
#include <variant>

struct Atom;
struct SquareBracketsTerm;
struct ParenthesesGroup;
struct SquareBracketsGroup;
struct CurlyBracketsGroup;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
// struct Association;

#define GROUP_ENTITIES ParenthesesGroup*, SquareBracketsGroup*, CurlyBracketsGroup*
#define POSTFIXES PostfixParenthesesGroup*, PostfixSquareBracketsGroup*

using ProgramWord = std::variant<
    Atom*,
    SquareBracketsTerm*,
    GROUP_ENTITIES,
    POSTFIXES
    // Association*
>;

using Word = std::variant<
    Atom*,
    /* no SquareBracketsTerm* here */
    GROUP_ENTITIES,
    POSTFIXES
    // Association*
>;

using WordWithoutAssociation = std::variant<
    Atom*,
    GROUP_ENTITIES,
    POSTFIXES
    /* no Association* here */
>;

MayFail<ProgramWord> consumeProgramWord(std::istringstream&);

MayFail<Word> consumeWord(std::istringstream&);

///////////////////////////////////////////////////////////

template <>
MayFail<ProgramWord> mayfail_cast<ProgramWord>(MayFail<Word> inputMayfail);

template <>
MayFail<Word> mayfail_cast<Word>(MayFail<ProgramWord> inputMayfail);

#endif // WORD_H
