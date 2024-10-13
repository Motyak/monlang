#ifndef SQUARE_BRACKETS_GROUP_H
#define SQUARE_BRACKETS_GROUP_H

#include <monlang/Term.h>
#include <monlang/common.h>

#include <sstream>
#include <vector>

struct PostfixSquareBracketsGroup;

struct SquareBracketsGroup {
    static const Sequence INITIATOR_SEQUENCE;
    static const Sequence CONTINUATOR_SEQUENCE;
    static const Sequence TERMINATOR_SEQUENCE;
    static const std::vector<char> RESERVED_CHARACTERS;

    std::vector<MayFail<Term>> terms;
};

MayFail<SquareBracketsGroup> consumeSquareBracketsGroupStrictly(std::istringstream& input, int indentLevel = 0);

using consumeSquareBracketsGroup_RetType = std::variant<
    MayFail<SquareBracketsGroup*>,
    MayFail<PostfixSquareBracketsGroup*>
>;
consumeSquareBracketsGroup_RetType consumeSquareBracketsGroup(std::istringstream& input, int indentLevel = 0);

#endif // SQUARE_BRACKETS_GROUP_H
