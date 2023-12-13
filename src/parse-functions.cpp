#include <parse-functions.h>
#include <variant-utils.h>

Program consumeProgram(std::istringstream& input) {

}

ProgramSentence consumeProgramSentence(std::istringstream& input) {

}

ProgramWord consumeProgramWord(std::istringstream& input) {

}

ProgramWordWithoutAssociation consumeProgramWordWithoutAssociation(std::istringstream& input) {

}

std::optional<Association> tryConsumeAssociation(std::istringstream& input) {

}

std::optional<ParenthesesGroup> tryConsumeParenthesesGroup(std::istringstream& input) {

}

std::optional<SquareBracketsGroup> tryConsumeSquareBracketsGroup(std::istringstream& input) {

}

std::optional<DoubleQuotesGroup> tryConsumeDoubleQuotesGroup(std::istringstream&) {

}

std::optional<CurlyBracketsGroup> tryConsumeCurlyBracketsGroup(std::istringstream& input) {

}

Atom consumeAtom(std::istringstream& input) {

}

void consumeSequence(std::string sequence, std::istringstream& input) {

}
