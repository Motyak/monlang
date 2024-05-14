#ifndef PROGRAM_SENTENCE_H
#define PROGRAM_SENTENCE_H

#include <Term.h>

#include <vector>
#include <sstream>

using ProgramSentence = Term;

MayFail<ProgramSentence> consumeProgramSentence(const std::vector<char>& terminatorCharacters, std::istringstream& input);

#endif // PROGRAM_SENTENCE_H
