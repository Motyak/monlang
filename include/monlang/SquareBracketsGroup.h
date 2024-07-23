#ifndef SQUARE_BRACKETS_GROUP_H
#define SQUARE_BRACKETS_GROUP_H

#include <monlang/Term.h>

#include <sstream>
#include <vector>

struct SquareBracketsGroup {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence CONTINUATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<MayFail<Term>> terms;
};

MayFail<SquareBracketsGroup> consumeSquareBracketsGroup(std::istringstream&);

#endif // SQUARE_BRACKETS_GROUP_H
