#include <Association.h>

const std::vector<char> Association::SEPARATOR_SEQUENCE = { ':' };

const std::vector<char> Association::RESERVED_CHARACTERS = {
    Association::SEPARATOR_SEQUENCE[0]
};
