#ifndef PROGRAM_SENTENCE_H
#define PROGRAM_SENTENCE_H

#include <monlang/ProgramWord.h>
#include <monlang/common.h>

#include <vector>
#include <sstream>

struct ProgramSentence {
    static const Sequence TAB_SEQUENCE;
    static const Sequence CONTINUATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<MayFail<ProgramWord>> programWords;
};

MayFail<ProgramSentence> consumeProgramSentence(std::istringstream& input, int indentLevel = 0);

#define INDENT_SEQUENCE() (indentLevel * ProgramSentence::TAB_SEQUENCE)

#if __has_include (<monlang/mayfail.hpp>)
#ifdef PROGRAM_SENTENCE_CPP
    // enable extern explicit instanciations..
    // ..for common.h 'mayfail' templates
    #include <monlang/mayfail.hpp>
#endif
#endif

#endif // PROGRAM_SENTENCE_H
