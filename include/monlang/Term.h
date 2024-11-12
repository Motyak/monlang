#ifndef TERM_H
#define TERM_H

#include <monlang/Word.h>
#include <monlang/common.h>

#include <utils/vec-utils.h>

#include <vector>
#include <sstream>

struct Term {
    static const Sequence CONTINUATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<Word> words;
};

template <>
struct MayFail_<Term> {
    std::vector<MayFail<Word_>> words;

    Term unwrap() const {
        return (Term)*this;
    }

    explicit operator Term() const {
        std::vector<Word> res;
        for (auto e: words) {
            res.push_back(unwrap_w(e.value()));
        }
        return Term{res};
    }
};

MayFail<MayFail_<Term>> consumeTerm(const std::vector<char>& terminatorCharacters, std::istringstream& input);

// for SquareBracketsTerm only (very special case)..
MayFail<MayFail_<Term>> consumeTerm(const Sequence& terminatorSequence, std::istringstream& input);

// (no terminator at all)
MayFail<MayFail_<Term>> consumeTerm(std::istringstream& input);

#endif // TERM_H
