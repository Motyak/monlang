#ifndef PROGRAM_SENTENCE_H
#define PROGRAM_SENTENCE_H

#include <monlang/ast/ProgramSentence.h>

#include <monlang/Word.h>

template<>
struct MayFail_<ProgramSentence> {
    std::vector<MayFail<ProgramWord_>> programWords;

    size_t _tokenLeadingNewlines = 0; // added by consumeProgram / consumeCurlyBracketsGroup
    size_t _tokenIndentSpaces = 0;
    size_t _tokenLen = 0;
    size_t _tokenTrailingNewlines = 0; // added by consumeCurlyBracketsGroup
    size_t _tokenId = 123456789;

    MayFail_() = default;
    explicit MayFail_(const std::vector<MayFail<ProgramWord_>>&);
    explicit MayFail_(const MayFail_<Term>&); // for casting term to sen

    explicit MayFail_(const ProgramSentence&);
    explicit operator ProgramSentence() const;
};

extern thread_local int indentLevel;
Sequence INDENT_SEQUENCE();

MayFail<MayFail_<ProgramSentence>> consumeProgramSentence(std::istringstream& input);

#endif // PROGRAM_SENTENCE_H
