#include <monlang/Association.h>
#include <monlang/common.h>

#include <utils/variant-utils.h>
#include <utils/assert-utils.h>

const Sequence Association::SEPARATOR_SEQUENCE = {':'};

const std::vector<char> Association::RESERVED_CHARACTERS = {
    sequenceFirstChar(Association::SEPARATOR_SEQUENCE).value()
};

///////////////////////////////////////////////////////////

Association::Association(AssociationLeftPart leftPart, Word rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<Association>::MayFail_(AssociationLeftPart leftPart, MayFail<Word_> rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<Association>::MayFail_(Association assoc) {
    this->leftPart = assoc.leftPart;
    this->rightPart = wrap_w(assoc.rightPart);
}

MayFail_<Association>::operator Association() const {
    return Association{leftPart, unwrap_w(rightPart.value())};
}
