#ifndef PROGRAM_SENTENCE_H
#define PROGRAM_SENTENCE_H

#include <Term.h>

#include <vector>
#include <sstream>

struct ProgramSentence : public Term {};

MayFail<ProgramSentence> consumeProgramSentence(std::istringstream& input);

#endif // PROGRAM_SENTENCE_H
