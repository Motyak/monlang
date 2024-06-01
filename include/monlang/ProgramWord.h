#ifndef PROGRAM_WORD_H
#define PROGRAM_WORD_H

#include <monlang/common.h>
#include <monlang/Word.h>

#include <vector>
#include <sstream>

struct ProgramWord : public Word {};

MayFail<ProgramWord> consumeProgramWord(std::istringstream&);

#endif // PROGRAM_WORD_H
