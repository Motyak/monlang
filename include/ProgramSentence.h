#ifndef PROGRAM_SENTENCE_H
#define PROGRAM_SENTENCE_H

#include <Term.h>

#include <vector>
#include <sstream>

using ProgramSentence = Term;

ProgramSentence consumeProgramSentence(std::istringstream&);

#endif // PROGRAM_SENTENCE_H
