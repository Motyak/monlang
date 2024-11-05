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

template <>
inline MayFail<ProgramWord> mayfail_cast<ProgramWord>(MayFail<Word> inputMayfail) {
    return inputMayfail.transform([](auto t) -> ProgramWord {return variant_cast(t);})
            .transform_error([](auto e) -> Malformed<ProgramWord> {return {variant_cast(e.val), e.err};});
}

static Word pw2w(ProgramWord pw) {
    return std::visit(overload{
        [](SquareBracketsTerm*) -> Word {SHOULD_NOT_HAPPEN();},
        [](auto word) -> Word {return word;},
    }, pw);
}

template <>
inline MayFail<Word> mayfail_cast<Word>(MayFail<ProgramWord> inputMayfail) {
    return inputMayfail.transform([](auto t) -> Word {return pw2w(t);})
        .transform_error([](auto e) -> Malformed<Word> {return {pw2w(e.val), e.err};});
}

#endif // WORD_H
