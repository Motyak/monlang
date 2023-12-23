#include <Quotation.h>

const std::vector<CharacterAppearance> Quotation::INITIATOR_SEQUENCE = { '"' };
const std::vector<CharacterAppearance> Quotation::TERMINATOR_SEQUENCE = { '"' };
const std::vector<CharacterAppearance> Quotation::ALT_INITIATOR_SEQUENCE = { '`', '`', '`' };
const std::vector<CharacterAppearance> Quotation::ALT_TERMINATOR_SEQUENCE = { '`', '`', '`' };

const std::vector<char> Quotation::RESERVED_CHARACTERS = {
    Quotation::INITIATOR_SEQUENCE[0],
    Quotation::TERMINATOR_SEQUENCE[0],
    Quotation::ALT_INITIATOR_SEQUENCE[0],
    Quotation::ALT_TERMINATOR_SEQUENCE[0],
};
