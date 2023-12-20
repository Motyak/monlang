#include <CurlyBracketsGroup.h>
#include <common.h>

const std::vector<char> CurlyBracketsGroup::INITIATOR_SEQUENCE = { '{' };
const std::vector<char> CurlyBracketsGroup::CONTINUATOR_SEQUENCE = { NEWLINE };
const std::vector<char> CurlyBracketsGroup::TERMINATOR_SEQUENCE = { '}' };

const std::vector<char> CurlyBracketsGroup::RESERVED_CHARACTERS = {
    CurlyBracketsGroup::INITIATOR_SEQUENCE[0],
    CurlyBracketsGroup::CONTINUATOR_SEQUENCE[0],
    CurlyBracketsGroup::TERMINATOR_SEQUENCE[0],
};
