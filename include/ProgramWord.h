#ifndef PROGRAM_WORD_H
#define PROGRAM_WORD_H

#include <Quotation.h>
#include <Atom.h>

#include <variant>
#include <sstream>

struct Association;
struct ParenthesesGroup;
struct SquareBracketsGroup;
struct CurlyBracketsGroup;
struct PostfixParenthesesGroup;
struct PostfixSquareBracketsGroup;
using ProgramWord = std::variant<Association*, ParenthesesGroup*, SquareBracketsGroup*, Quotation, CurlyBracketsGroup*, Atom>;

using ProgramWordWithoutAssociation = std::variant<ParenthesesGroup*, SquareBracketsGroup*, Quotation, CurlyBracketsGroup*, Atom>;

ProgramWord consumeProgramWord(std::istringstream&);
ProgramWordWithoutAssociation consumeProgramWordWithoutAssociation(std::istringstream&);

#endif // PROGRAM_WORD_H
