#ifndef PROGRAM_WORD_H
#define PROGRAM_WORD_H

#include <monlang/Word.h>
#include <monlang/common.h>

#include <sstream>

struct ProgramWord : public Word {};

MayFail<ProgramWord> consumeProgramWord(std::istringstream& input, int indentLevel = 0);

#endif // PROGRAM_WORD_H
