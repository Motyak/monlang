#ifndef PARSE_FUNCTIONS_H
#define PARSE_FUNCTIONS_H

#include <grammar-structs.h>

#include <optional>
#include <sstream>

Program consumeProgram(std::istringstream&);

ProgramSentence consumeProgramSentence(std::istringstream&);

ProgramWord consumeProgramWord(std::istringstream&);
ProgramWordWithoutAssociation consumeProgramWordWithoutAssociation(std::istringstream&);

std::optional<ParenthesesGroup*> tryConsumeParenthesesGroup(std::istringstream&);
std::optional<SquareBracketsGroup*> tryConsumeSquareBracketsGroup(std::istringstream&);
std::optional<Quotation> tryConsumeQuotation(std::istringstream&);
std::optional<CurlyBracketsGroup*> tryConsumeCurlyBracketsGroup(std::istringstream&);
std::optional<Association*> tryConsumeAssociation(std::istringstream&);
Quoted consumeQuoted(std::istringstream&);
Atom consumeAtom(std::istringstream&);

void consumeSequence(std::vector<char> sequence, std::istringstream& input);

#endif // PARSE_FUNCTIONS_H
