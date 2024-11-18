#ifndef PROGRAM_SENTENCE_H
#define PROGRAM_SENTENCE_H

#include <monlang/ast/ProgramSentence.h>

#include <monlang/Word.h>

template<>
struct MayFail_<ProgramSentence> {
    std::vector<MayFail<ProgramWord_>> programWords;

    MayFail_() = default;
    explicit MayFail_(std::vector<MayFail<ProgramWord_>>);

    explicit MayFail_(ProgramSentence);
    explicit operator ProgramSentence() const;
};

MayFail<MayFail_<ProgramSentence>> consumeProgramSentence(std::istringstream& input, int indentLevel = 0);

#define INDENT_SEQUENCE() (indentLevel * ProgramSentence::TAB_SEQUENCE)

template <>
ProgramSentence unwrap(const MayFail_<ProgramSentence>&);

template <>
MayFail_<ProgramSentence> wrap(const ProgramSentence&);

#endif // PROGRAM_SENTENCE_H
