#ifndef WORD_H
#define WORD_H

#include <monlang/ast/Word.h>

#include <monlang/common.h>

#define GROUP_ENTITIES_ MayFail_<ParenthesesGroup>*, MayFail_<SquareBracketsGroup>*, MayFail_<CurlyBracketsGroup>*
#define POSTFIXES_ MayFail_<PostfixParenthesesGroup>*, MayFail_<PostfixSquareBracketsGroup>*

using ProgramWord_ = std::variant<
    Atom*,
    MayFail_<Quotation>*,
    MayFail_<SquareBracketsTerm>*,
    GROUP_ENTITIES_,
    POSTFIXES_,
    MayFail_<Association>*
>;

using Word_ = std::variant<
    Atom*,
    MayFail_<Quotation>*,
    /* no MayFail_<SquareBracketsTerm>* here */
    GROUP_ENTITIES_,
    POSTFIXES_,
    MayFail_<Association>*
>;

MayFail<ProgramWord_> consumeProgramWord(std::istringstream&);

MayFail<Word_> consumeWord(std::istringstream&);

///////////////////////////////////////////////////////////

ProgramWord unwrap_pw(const ProgramWord_&);
ProgramWord_ wrap_pw(const ProgramWord&);
Word unwrap_w(const Word_&);
Word_ wrap_w(const Word&);

template <>
MayFail<ProgramWord_> mayfail_cast<ProgramWord_>(const MayFail<Word_>& inputMayfail);

template <>
MayFail<Word_> mayfail_cast<Word_>(const MayFail<ProgramWord_>& inputMayfail);

#endif // WORD_H
