#ifndef MULTILINE_SQUARE_BRACKETS_GROUP_H
#define MULTILINE_SQUARE_BRACKETS_GROUP_H

#include <monlang/ast/MultilineSquareBracketsGroup.h>

#include <monlang/ProgramSentence.h>

template <>
struct MayFail_<MultilineSquareBracketsGroup> {
    std::vector<MayFail<MayFail_<ProgramSentence>>> sentences;

    size_t _tokenLen = 0;
    size_t _tokenId = 123456789;
    MayFail_() = default;
    explicit MayFail_(const std::vector<MayFail<MayFail_<ProgramSentence>>>&);

    explicit MayFail_(const MultilineSquareBracketsGroup&);
    explicit operator MultilineSquareBracketsGroup() const;
};

MayFail<MayFail_<MultilineSquareBracketsGroup>> consumeMultilineSquareBracketsGroup(std::istringstream&);

#endif // MULTILINE_SQUARE_BRACKETS_GROUP_H
