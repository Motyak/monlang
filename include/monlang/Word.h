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
struct Association;

#define GROUP_ENTITIES ParenthesesGroup*, SquareBracketsGroup*, CurlyBracketsGroup*
#define POSTFIXES PostfixParenthesesGroup*, PostfixSquareBracketsGroup*

using ProgramWord = std::variant<
    Atom*,
    SquareBracketsTerm*,
    GROUP_ENTITIES,
    POSTFIXES,
    Association*
>;

using Word = std::variant<
    Atom*,
    /* no SquareBracketsTerm* here */
    GROUP_ENTITIES,
    POSTFIXES,
    Association*
>;

#define GROUP_ENTITIES_ MayFail_<ParenthesesGroup>*, MayFail_<SquareBracketsGroup>*, MayFail_<CurlyBracketsGroup>*
#define POSTFIXES_ MayFail_<PostfixParenthesesGroup>*, MayFail_<PostfixSquareBracketsGroup>*

using ProgramWord_ = std::variant<
    Atom*,
    MayFail_<SquareBracketsTerm>*,
    GROUP_ENTITIES_,
    POSTFIXES_,
    MayFail_<Association>*
>;

using Word_ = std::variant<
    Atom*,
    /* no MayFail_<SquareBracketsTerm>* here */
    GROUP_ENTITIES_,
    POSTFIXES_,
    MayFail_<Association>*
>;

MayFail<ProgramWord_> consumeProgramWord(std::istringstream&);

MayFail<Word_> consumeWord(std::istringstream&);

///////////////////////////////////////////////////////////

ProgramWord unwrap_pw(ProgramWord_);
ProgramWord_ wrap_pw(ProgramWord);
Word unwrap_w(Word_);
Word_ wrap_w(Word);

template <>
MayFail<ProgramWord_> mayfail_cast<ProgramWord_>(MayFail<Word_> inputMayfail);

template <>
MayFail<Word_> mayfail_cast<Word_>(MayFail<ProgramWord_> inputMayfail);

#endif // WORD_H
