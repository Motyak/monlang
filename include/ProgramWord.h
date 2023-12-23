#ifndef PROGRAM_WORD_H
#define PROGRAM_WORD_H

#include <Quotation.h>
#include <Atom.h>

#include <variant>
#include <sstream>

struct Association;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
struct ParenthesesGroup;
struct SquareBracketsGroup;
struct CurlyBracketsGroup;
using ProgramWord = std::variant<Association*, PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, ParenthesesGroup*, SquareBracketsGroup*, Quotation, CurlyBracketsGroup*, Atom>;

using ProgramWordWithoutAssociation = std::variant<PostfixParenthesesGroup*, PostfixSquareBracketsGroup*, ParenthesesGroup*, SquareBracketsGroup*, Quotation, CurlyBracketsGroup*, Atom>;

ProgramWord consumeProgramWord(std::istringstream&);
// ProgramWordWithoutAssociation consumeProgramWordWithoutAssociation(std::istringstream&);

#endif // PROGRAM_WORD_H
