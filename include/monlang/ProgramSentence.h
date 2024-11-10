#ifndef PROGRAM_SENTENCE_H
#define PROGRAM_SENTENCE_H

#include <monlang/Word.h>
#include <monlang/common.h>

#include <vector>
#include <sstream>

struct ProgramSentence {
    static const Sequence TAB_SEQUENCE;
    static const Sequence CONTINUATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<ProgramWord> programWords;
};

template <>
struct MayFail<ProgramSentence> : public MayFail<void> {
    std::vector<MayFail<ProgramWord>> programWords;

    MayFail() = default;
    MayFail(ProgramSentence sentence) : MayFail<void>() {
        std::vector<MayFail<ProgramWord>> programWords;
        for (auto pw: sentence.programWords) {
            programWords.push_back(MayFail<ProgramWord>(pw));
        }
        this->programWords = programWords;
    }
    MayFail(ProgramSentence sentence, Error err) : MayFail<void>(err){
        std::vector<MayFail<ProgramWord>> programWords;
        for (auto pw: sentence.programWords) {
            programWords.push_back(MayFail<ProgramWord>(pw));
        }
        this->programWords = programWords;
    }
};

MayFail<ProgramSentence> consumeProgramSentence(std::istringstream& input, int indentLevel = 0);

#define INDENT_SEQUENCE() (indentLevel * ProgramSentence::TAB_SEQUENCE)

#endif // PROGRAM_SENTENCE_H
