#ifndef PARSE_FUNCTIONS_H
#define PARSE_FUNCTIONS_H

#include <grammar-structs.h>

#include <optional>

Program parseProgram(const std::string& input);

ProgramSentence* parseProgramSentence(const std::string&);

ProgramWord parseProgramWord(const std::string&);
ProgramWordWithoutAssociation parseProgramWordWithoutAssociation(const std::string&);

std::optional<Association*> tryParseAssociation(const std::string&);
std::optional<ParenthesesGroup*> tryParseParenthesesGroup(const std::string&);
std::optional<SquareBracketsGroup*> tryParseSquareBracketsGroup(const std::string&);
std::optional<CurlyBracketsGroup*> tryParseCurlyBracketsGroup(const std::string&);
Atom* parseAtom(const std::string&);

#endif // PARSE_FUNCTIONS_H
