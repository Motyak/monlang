#ifndef TERM_H
#define TERM_H

#include <monlang/ast/Term.h>

#include <monlang/Word.h>
#include <monlang/common.h>

template <>
struct MayFail_<Term> {
    std::vector<MayFail<Word_>> words;

    MayFail_() = default;
    explicit MayFail_(std::vector<MayFail<Word_>>);

    explicit MayFail_(Term);
    explicit operator Term() const;
};

MayFail<MayFail_<Term>> consumeTerm(const std::vector<char>& terminatorCharacters, std::istringstream& input);

// for SquareBracketsTerm only (very special case)..
MayFail<MayFail_<Term>> consumeTerm(const Sequence& terminatorSequence, std::istringstream& input);

// (no terminator at all)
MayFail<MayFail_<Term>> consumeTerm(std::istringstream& input);

#endif // TERM_H
