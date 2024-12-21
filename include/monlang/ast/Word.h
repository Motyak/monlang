#ifndef AST_WORD_H
#define AST_WORD_H

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

bool holds_word(const ProgramWord&);
Word get_word(const ProgramWord&);
// ProgramWord deepcopy(const ProgramWord&); // TODO: should we add this ?

struct Term;
Term as_term(const Word&);

#endif // AST_WORD_H
