#ifndef AST_WORD_H
#define AST_WORD_H

#include <variant>

struct Atom;
struct Quotation;
struct SquareBracketsTerm;
struct ParenthesesGroup;
struct SquareBracketsGroup;
struct MultilineSquareBracketsGroup;
struct CurlyBracketsGroup;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
struct Path;
struct Association;

#define GROUP_ENTITIES ParenthesesGroup*, SquareBracketsGroup* \
        , CurlyBracketsGroup*, MultilineSquareBracketsGroup*
#define POSTFIXES PostfixParenthesesGroup*, PostfixSquareBracketsGroup*

using ProgramWord = std::variant<
    Atom*,
    Quotation*,
    SquareBracketsTerm*,
    GROUP_ENTITIES,
    POSTFIXES,
    Path*,
    Association*
>;

using Word = std::variant<
    Atom*,
    Quotation*,
    /* no SquareBracketsTerm* here */
    GROUP_ENTITIES,
    POSTFIXES,
    Path*,
    Association*
>;

bool holds_word(const ProgramWord&);
Word get_word(const ProgramWord&);
// ProgramWord deepcopy(const ProgramWord&); // TODO: should we add this ?

struct Term;
Term as_term(const Word&);
Word as_word(const Term&);

#endif // AST_WORD_H
