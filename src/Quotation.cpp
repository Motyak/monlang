#include <Quotation.h>

const std::vector<CharacterAppearance> Quotation::INITIATOR_SEQUENCE = { '"' };
const std::vector<CharacterAppearance> Quotation::TERMINATOR_SEQUENCE = { {BACKSLASH, 0}, '"' };
const std::vector<CharacterAppearance> Quotation::ALT_INITIATOR_SEQUENCE = { {'`', 3} };
const std::vector<CharacterAppearance> Quotation::ALT_TERMINATOR_SEQUENCE = { {BACKSLASH, 0}, {'`', 3} };

const std::vector<char> Quotation::RESERVED_CHARACTERS = {
    firstChar(INITIATOR_SEQUENCE),
    firstChar(TERMINATOR_SEQUENCE),
    firstChar(ALT_INITIATOR_SEQUENCE),
    firstChar(ALT_TERMINATOR_SEQUENCE),
};
