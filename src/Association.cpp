#include <monlang/Association.h>
#include <monlang/common.h>

#include <utils/variant-utils.h>
#include <utils/assert-utils.h>

const Sequence Association::SEPARATOR_SEQUENCE = {':'};

const std::vector<char> Association::RESERVED_CHARACTERS = {
    sequenceFirstChar(Association::SEPARATOR_SEQUENCE).value()
};

AssociationLeftPart unwrap_assoc_left_part(AssociationLeftPart_ alp_) {
    return std::visit(overload{
        [](Association*) -> AssociationLeftPart {SHOULD_NOT_HAPPEN();},
        [](auto word) -> AssociationLeftPart {return word;}
    }, unwrap_w(variant_cast(alp_)));
}
