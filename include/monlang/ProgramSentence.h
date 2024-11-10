#ifndef PROGRAM_SENTENCE_H
#define PROGRAM_SENTENCE_H

#include <monlang/Word.h>
#include <monlang/common.h>

#include <utils/vec-utils.h>

#include <vector>
#include <sstream>

struct ProgramSentence {
    static const Sequence TAB_SEQUENCE;
    static const Sequence CONTINUATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<ProgramWord> programWords;
};

template<>
struct MayFail_<ProgramSentence> {
    std::vector<MayFail<ProgramWord>> programWords;

    // MayFail_<ProgramSentence>(ProgramSentence sentence) 
    //     : programWords(vec_cast<MayFail<ProgramWord>>(sentence.programWords))
    // {}
    explicit operator ProgramSentence() const {
        return ProgramSentence{vec_convert<ProgramWord>(programWords)};
    }
};

MayFail<MayFail_<ProgramSentence>> consumeProgramSentence(std::istringstream& input, int indentLevel = 0);

#define INDENT_SEQUENCE() (indentLevel * ProgramSentence::TAB_SEQUENCE)

#endif // PROGRAM_SENTENCE_H
