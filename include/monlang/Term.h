#ifndef TERM_H
#define TERM_H

#include <monlang/Word.h>
#include <monlang/common.h>

#include <vector>
#include <sstream>

struct Term {
    static const Sequence CONTINUATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<MayFail<Word>> words;
};

MayFail<Term> consumeTerm(const std::vector<char>& terminatorCharacters, std::istringstream& input);

#if __has_include (<monlang/mayfail.hpp>)
#ifdef TERM_CPP
    // enable extern explicit instanciations..
    // ..for common.h 'mayfail' templates
    #include <monlang/mayfail.hpp>
#endif
#endif

#endif // TERM_H
