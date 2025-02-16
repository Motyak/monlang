#include <monlang/Association.h>
#include <monlang/common.h>

#include <utils/variant-utils.h>
#include <utils/assert-utils.h>

const Sequence Association::SEPARATOR_SEQUENCE = {':', {':', 0_}};

const std::vector<char> Association::RESERVED_CHARACTERS = {
    sequenceFirstChar(Association::SEPARATOR_SEQUENCE).value()
};

///////////////////////////////////////////////////////////

Association::Association(const AssociationLeftPart& leftPart, const Word& rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<Association>::MayFail_(const AssociationLeftPart& leftPart, const MayFail<Word_>& rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<Association>::MayFail_(const Association& assoc) {
    this->leftPart = assoc.leftPart;
    this->rightPart = wrap_w(assoc.rightPart);
    this->_tokenLen = assoc._tokenLen;
}

MayFail_<Association>::operator Association() const {
    auto assoc = Association{leftPart, unwrap_w(rightPart.value())};
    assoc._tokenLen = this->_tokenLen;
    return assoc;
}
