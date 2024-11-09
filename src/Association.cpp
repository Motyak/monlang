#include <monlang/Association.h>
#include <monlang/common.h>

#include <utils/variant-utils.h>

const Sequence Association::SEPARATOR_SEQUENCE = {':'};

const std::vector<char> Association::RESERVED_CHARACTERS = {
    sequenceFirstChar(Association::SEPARATOR_SEQUENCE).value()
};
