#include <Association.h>

const std::vector<CharacterAppearance> Association::SEPARATOR_SEQUENCE = { ':' };

const std::vector<char> Association::RESERVED_CHARACTERS = {
    firstChar(SEPARATOR_SEQUENCE)
};
