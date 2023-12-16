#ifndef PROGRAM_WORD_H
#define PROGRAM_WORD_H

#include <Quotation.h>
#include <Atom.h>

#include <variant>

struct Association;
struct ParenthesesGroup;
struct SquareBracketsGroup;
struct CurlyBracketsGroup;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
using ProgramWord = std::variant<Association*, ParenthesesGroup*, SquareBracketsGroup*, Quotation, CurlyBracketsGroup*, Atom>;

using ProgramWordWithoutAssociation = std::variant<ParenthesesGroup*, SquareBracketsGroup*, Quotation, CurlyBracketsGroup*, Atom>;

#endif // PROGRAM_WORD_H
