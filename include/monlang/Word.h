#ifndef WORD_H
#define WORD_H

#include <monlang/common.h>

#include <sstream>
#include <variant>

struct Atom;
struct SquareBracketsTerm;
struct ParenthesesGroup;
struct SquareBracketsGroup;
struct CurlyBracketsGroup;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;

using Word = std::variant<
    Atom*,
    SquareBracketsTerm*,
    ParenthesesGroup*, SquareBracketsGroup*, CurlyBracketsGroup*,
    PostfixParenthesesGroup*, PostfixSquareBracketsGroup*
>;

MayFail<Word> consumeWord(std::istringstream&);

// PROGRAM WORD ///////////////////////////////////////////

struct ProgramWord : public Word {};
MayFail<ProgramWord> consumeProgramWord(std::istringstream&);

#endif // WORD_H
