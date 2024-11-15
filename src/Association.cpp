#include <monlang/Association.h>
#include <monlang/common.h>

#include <utils/variant-utils.h>
#include <utils/assert-utils.h>

const Sequence Association::SEPARATOR_SEQUENCE = {':'};

const std::vector<char> Association::RESERVED_CHARACTERS = {
    sequenceFirstChar(Association::SEPARATOR_SEQUENCE).value()
};

///////////////////////////////////////////////////////////

MayFail_<Association> Association::wrap() const {
    return MayFail_<Association>{leftPart, wrap_w(rightPart)};
}

MayFail_<Association>::MayFail_(AssociationLeftPart leftPart, MayFail<Word_> rightPart) : leftPart(leftPart), rightPart(rightPart){}

MayFail_<Association>::MayFail_(Association assoc) : MayFail_(assoc.wrap()){}

MayFail_<Association>::operator Association() const {
    return Association{leftPart, unwrap_w(rightPart.value())};
}

Association MayFail_<Association>::unwrap() const {
    return (Association)*this;
}
